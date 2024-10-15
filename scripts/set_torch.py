#!/usr/bin/env python3
import os
import subprocess
import sys
import re

def get_torch_install_path():
    result = subprocess.run(['pip', 'show', 'torch'], stdout=subprocess.PIPE, text=True)
    if result.returncode == 0:
        output = result.stdout
        match = re.search(r'Location: (.*)', output)
        if match:
            return match.group(1)
    return None

def modify_files(action, torch_install_path):
    file_paths = [
        os.path.join(torch_install_path, "torch/include/ATen/core/dispatch/OperatorEntry.h"),
        os.path.join(torch_install_path, "torch/include/ATen/core/dispatch/Dispatcher.h")
    ]

    for file_path in file_paths:
        if not os.path.exists(file_path):
            return

    pattern = re.compile(r'^(\s*)private:', re.MULTILINE)

    for file_path in file_paths:
        with open(file_path, 'r') as file:
            content = file.read()

        if action == 'install':
            content = pattern.sub(r'\1//private:', content)
        elif action == 'uninstall':
            content = content.replace('//private:', 'private:')
        else:
            return

        with open(file_path, 'w') as file:
            file.write(content)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(1)

    action = sys.argv[1].lower()
    torch_install_path = get_torch_install_path()
    
    if torch_install_path == None:
        raise RuntimeError("get torch_install_path error!!")

    modify_files(action, torch_install_path)