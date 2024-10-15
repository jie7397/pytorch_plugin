#include "hook_register.h"

namespace hook
{

    RegisterHook &RegisterHook::getInstance()
    {
        static RegisterHook instance;
        return instance;
    }

    void RegisterHook::Register(std::string lib_name, std::string symbol_name, void *func)
    {
        auto it = this->hook_config.find(lib_name);
        if (it != this->hook_config.end())
        {
            it->second.insert({symbol_name, func});
        }
        else
        {
            std::unordered_map<std::string, void *> symbol_set;
            symbol_set.insert({symbol_name, func});
            hook_config[lib_name] = symbol_set;
        }
    }

    void *RegisterHook::getFunc(std::string lib_name, std::string symbol_name)
    {
        auto it = this->hook_config.find(lib_name);

        if (it != this->hook_config.end())
        {
            auto func_it = it->second.find(symbol_name);
            if (it != this->hook_config.end())
            {
                func_it->second;
            }
        }
        return nullptr;
    }

    bool RegisterHook::matchHookLib(std::string lib_name)
    {

        auto it = this->hook_config.find(lib_name);

        if (it != this->hook_config.end())
        {
            return true;
        }

        return false;
    }

}