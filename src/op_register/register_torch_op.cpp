#include "regitster_torch_op.h"


void RegisterTorchOp::Register(const char* name, func_wrapper_ptr function) {
    functionMap[name] = std::move(function);
}

RegisterTorchOp& RegisterTorchOp::getInstance() {
    static RegisterTorchOp instance;
    return instance;
}