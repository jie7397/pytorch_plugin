workspace_dir=$(dirname "$(dirname "$(realpath "$0")")")

pushd ${workspace_dir}

pytest tests/

popd