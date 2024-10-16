# pytorch_plugin
dynamic modify pytorch

## build
```bash
bash scripts/build.sh
```

## deregister all torch aten op
```python
torch_plugin._C.deregister_all_op("aten")
torch_plugin._C.deregister_all_op("torchvision", {"torchvision::_deform_conv2d_backward",})
torch_plugin._C.list_all_cuda_kernel()
```

## log
```bash
#log level : [warn/info/debug]
export export SPDLOG_LEVEL=warn
```