import torch_plugin
import torchvision

torch_plugin._C.deregister_all_op("aten")
# torch_plugin._C.deregister_all_op("torchvision", {"torchvision::_deform_conv2d_backward",})
torch_plugin._C.list_all_cuda_kernel()