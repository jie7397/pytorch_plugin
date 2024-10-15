#ifndef HOOK_REGISTER_H
#define HOOK_REGISTER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "spdlog/spdlog.h"

namespace hook
{

    class RegisterHook
    {

    public:
        static RegisterHook &getInstance();

        void Register(std::string lib_name, std::string symbol_name, void *func);
        std::unordered_set<std::string> getHookSymol(std::string lib_name);
        void *getFunc(std::string lib_name, std::string symbol_name);
        bool matchHookLib(std::string lib_name);

    private:
        RegisterHook() = default;
        ~RegisterHook() = default;

        RegisterHook(const RegisterHook &) = delete;
        RegisterHook &operator=(const RegisterHook &) = delete;

        std::unordered_map<std::string, std::unordered_map<std::string, void *>> hook_config;
    };

    void hook();

}

#endif