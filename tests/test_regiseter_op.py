import pytest
import os
import torch
import torch_plugin

def test_regiseter_op_demo():
    os.environ["USE_TORCH_OP"] = "1"
    a = torch.ones(2)
    b = torch.ones(2)
    c = torch_plugin.torch_plugin_C.add_tensors(a, b)
    assert c.allclose(torch.zeros(2))

