
#!/bin/bash

workspace_dir=$(dirname "$(dirname "$(realpath "$0")")")

pushd ${workspace_dir}
python setup.py bdist_wheel
pip install dist/torch_plugin-0.0.1-cp310-cp310-macosx_13_0_arm64.whl  --force-reinstall --no-deps

popd