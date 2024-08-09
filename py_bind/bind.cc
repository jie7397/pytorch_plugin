#include <pybind11/pybind11.h>
#include <torch/torch.h>
#include <torch/extension.h>
#include <iostream>

#include "regitster_torch_op.h"

namespace py = pybind11;

at::Tensor add_tensors(const at::Tensor & tensor1, const at::Tensor & tensor2) {
    const char* use_xtrans_op = std::getenv("USE_TORCH_OP");

    if (use_xtrans_op && std::string(use_xtrans_op) == "1") {
        auto func = GET_TORCH_OP("sub_tensors", decltype(add_tensors));
        return func(tensor1, tensor2);
    }
    return tensor1 + tensor2;
}

at::Tensor sub_tensors(const at::Tensor & tensor1, const at::Tensor & tensor2) {
    return tensor1 - tensor2;
}

REGISTER_TORCH_OP(sub_tensors, sub_tensors);


PYBIND11_MODULE(torch_plugin_C, m) {
    m.def("add_tensors", &add_tensors, "A function that adds two tensors");
}