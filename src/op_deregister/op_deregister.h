#include <torch/library.h>
#include <ATen/core/dispatch/Dispatcher.h>
#include "spdlog/spdlog.h"

namespace TorchPlugin
{

    bool deregister_all_op();

} // namespace torch_plugin
