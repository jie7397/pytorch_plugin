
#!/bin/bash

workspace_dir=$(dirname "$(dirname "$(realpath "$0")")")

pushd ${workspace_dir}
git submodule sync
git submodule update --init --recursive

python setup.py bdist_wheel
pip install dist/torch_plugin-*.whl  --force-reinstall --no-deps

popd