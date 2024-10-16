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

    bool deregister_all_op(std::string ns, std::unordered_set<std::string> ignore_lsit = {})
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
            if (auto search = ignore_lsit.find(op.name); search != ignore_lsit.end()) {
                SPDLOG_INFO("SKIP DEREGISTIER OP: {}", ns, op.name);
                continue;
            }

            auto op_handle = c10::Dispatcher::singleton().findOp(op);
            if (op_handle->hasKernelForDispatchKey(c10::DispatchKey::CUDA) && get_namespace(op.name) == ns)
            {

                op_handle = const_cast<c10::OperatorHandle &>(op_handle.value());
                deregister_op(op_handle->operatorDef_->op, c10::DispatchKey::CUDA, op_handle.value(), op);
                SPDLOG_INFO("DEREGISTIER OP: {} success", ns, op.name);
            }
        }

        c10::Dispatcher::singleton().guard_->alive.store(false);

        bool res = true;

        for (auto op : op_names)
        {
            if (auto search = ignore_lsit.find(op.name); search != ignore_lsit.end())
            {
                continue;
            }
    
            auto op_handle = c10::Dispatcher::singleton().findOp(op);
            if (op_handle->hasKernelForDispatchKey(c10::DispatchKey::CUDA) && get_namespace(op.name) == ns)
            {
                SPDLOG_ERROR("OP {} deregister Fail !!!", op.name);
                res = false;
            }
        }

        return res;
    }

    void list_all_cuda_kernel()
    {
        auto op_names = c10::Dispatcher::singleton().getAllOpNames();

        for (auto op : op_names)
        {

            auto op_handle = c10::Dispatcher::singleton().findOp(op);
            if (op_handle->hasKernelForDispatchKey(c10::DispatchKey::CUDA))
            {
                SPDLOG_INFO("OP : {}, has CUDA kernel", op.name);
            }
        }

    }

}
