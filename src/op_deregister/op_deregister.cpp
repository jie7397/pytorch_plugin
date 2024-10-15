#include "op_deregister.h"

namespace TorchPlugin
{

    void deregister_op(c10::impl::OperatorEntry &member_map, const c10::DispatchKey &dk, c10::OperatorHandle &op, c10::OperatorName &op_name)
    {
        auto it = member_map.kernels_.find(dk);
        if (it != member_map.kernels_.end())
        {
            auto &k = it->second;
            c10::Dispatcher::singleton().deregisterImpl_(
                op,
                op_name,
                c10::DispatchKey::CUDA,
                k.begin());
        }
    }

    bool deregister_all_op()
    {
        auto get_namespace = [](std::string s)
        {
            std::string first_half = "";
            std::size_t pos = s.find("::");
            if (pos != std::string::npos)
            {
                first_half = s.substr(0, pos);
            }
            return first_half;
        };

        auto op_names = c10::Dispatcher::singleton().getAllOpNames();

        for (auto op : op_names)
        {

            auto op_handle = c10::Dispatcher::singleton().findOp(op);
            if (op_handle->hasKernelForDispatchKey(c10::DispatchKey::CUDA) && get_namespace(op.name) == "aten")
            {

                op_handle = const_cast<c10::OperatorHandle &>(op_handle.value());
                deregister_op(op_handle->operatorDef_->op, c10::DispatchKey::CUDA, op_handle.value(), op);
                SPDLOG_INFO("deregister {} success", op.name);
            }
        }

        c10::Dispatcher::singleton().guard_->alive.store(false);

        bool res = true;

        for (auto op : op_names)
        {
            auto op_handle = c10::Dispatcher::singleton().findOp(op);
            if (op_handle->hasKernelForDispatchKey(c10::DispatchKey::CUDA) && get_namespace(op.name) == "aten")
            {
                SPDLOG_ERROR("OP : {} deregister fail !!!", op.name);
                res = false;
            }
        }

        return res;
    }

}
