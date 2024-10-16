#include <torch/library.h>
#include <ATen/core/dispatch/Dispatcher.h>
#include "spdlog/spdlog.h"

namespace TorchPlugin
{

    bool deregister_all_op(std::string ns, std::unordered_set<std::string> ignore_lsit);
    void list_all_cuda_kernel();
} // namespace torch_plugin
