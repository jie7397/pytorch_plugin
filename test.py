import os
os.environ["USE_XTRANS_OP"] = "1"


import torch_plugin
import torch


a = torch.ones(2)
b = torch.ones(2) 

print(a)
print(b)
c = torch_plugin.torch_plugin_C.add_tensors(a, b)

print(c)


