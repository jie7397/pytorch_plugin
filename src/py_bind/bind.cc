#include <pybind11/pybind11.h>
#include <torch/torch.h>
#include <torch/extension.h>
#include <torch/library.h>

#include "regitster_torch_op.h"
#include "op_deregister.h"
#include "hook_register.h"

#include "spdlog/spdlog.h"

namespace py = pybind11;

template <std::vector<c10::RegistrationHandleRAII> torch::Library::*MEMBER_INT_PTR>
struct GenerateThiefFunction
{

    friend void steal_from(torch::Library &victim_object)
    {
        std::cout << (victim_object.*MEMBER_INT_PTR).size() << std::endl;
    }
};

template struct GenerateThiefFunction<&torch::Library::registrars_>;

void steal_from(torch::Library &victim_object);

void hook_torch()
{
    auto lib = MAKE_TORCH_LIBRARY_IMPL(aten, CUDA);
    steal_from(lib);
}

at::Tensor add_tensors(const at::Tensor &tensor1, const at::Tensor &tensor2)
{
    const char *use_xtrans_op = std::getenv("USE_TORCH_OP");

    if (use_xtrans_op && std::string(use_xtrans_op) == "1")
    {
        auto func = GET_TORCH_OP("sub_tensors", decltype(add_tensors));
        return func(tensor1, tensor2);
    }
    return tensor1 + tensor2;
}

at::Tensor sub_tensors(const at::Tensor &tensor1, const at::Tensor &tensor2)
{
    return tensor1 - tensor2;
}

REGISTER_TORCH_OP(sub_tensors, sub_tensors);

PYBIND11_MODULE(torch_plugin_C, m)
{
    m.def("add_tensors", &add_tensors, "A function that adds two tensors");
    // m.def("hook_torch", &hook_torch, "hook torch");

    m.def("hook", &hook::hook, "hook cuda func");

    m.def("deregister_all_op", &TorchPlugin::deregister_all_op,
        py::arg("ns"), py::arg("ignore_list") = std::unordered_set<std::string>{},
        "deregister cuda kernel");

    m.def("list_all_cuda_kernel", &TorchPlugin::list_all_cuda_kernel, "list cuda kernel");
}