#include "regitster_xtrans.h"


#define REGISTER_XTRANS_OP(name, func) \
    static bool _registered_##name = [](){ \
        RegisterTorchOp::getInstance().Register(#name, \
        std::make_unique<FunctionWrapper<std::function<decltype(func)>>>(func)); \
        return true; \
    }();

#define GET_XTRANS_OP(name, FuncType) \
    RegisterTorchOp::getInstance().get<std::function<FuncType>>(name)