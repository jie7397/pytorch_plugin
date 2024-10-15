# pytorch_plugin
dynamic modify pytorch

## build
```bash
bash scripts/build.sh
```

## deregister all torch aten op
```python
import torch_plugin
torch_plugin._C.deregister_all_op()
```

## log
```bash
#log level : [warn/info/debug]
export export SPDLOG_LEVEL=warn
```