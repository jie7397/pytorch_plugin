#ifndef REGISTER_TORCH_OP_H
#define REGISTER_TORCH_OP_H

#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <stdexcept>
#include "log.h"


#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

class EXPORT BaseFunctionWrapper {
public:
    virtual ~BaseFunctionWrapper() = default;
};

template<typename FuncType>
class EXPORT FunctionWrapper : public BaseFunctionWrapper {
public:
    explicit FunctionWrapper(FuncType func) : func_(func) {}

    FuncType get_func() {
        return func_;
    }

private:
    FuncType func_;
};

class EXPORT RegisterTorchOp {
public:
    using func_wrapper_ptr = std::unique_ptr<BaseFunctionWrapper>;

    void Register(const char* name, func_wrapper_ptr function);

    template<typename FuncType>
    FuncType get(const char* name) {
        auto it = functionMap.find(name);
        
        if (it != functionMap.end()) {
            auto wrapper = dynamic_cast<FunctionWrapper<FuncType>*>(it->second.get());
            if (wrapper) {
                return wrapper->get_func();
            }
        }
            return nullptr;

    }

    static RegisterTorchOp& getInstance();

private:
    RegisterTorchOp() = default;
    ~RegisterTorchOp() = default;

    RegisterTorchOp(const RegisterTorchOp&) = delete;
    RegisterTorchOp& operator=(const RegisterTorchOp&) = delete;

    std::unordered_map<std::string, func_wrapper_ptr> functionMap;
};

#define REGISTER_TORCH_OP(name, func) \
    static bool _registered_##name = [](){ \
        RegisterTorchOp::getInstance().Register(#name, \
        std::make_unique<FunctionWrapper<std::function<decltype(func)>>>(func)); \
        return true; \
    }();

#define GET_TORCH_OP(name, FuncType) \
    RegisterTorchOp::getInstance().get<std::function<FuncType>>(name)
#endif