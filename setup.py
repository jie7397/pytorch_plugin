import os
import re
import subprocess
import sys
import platform
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from shutil import copyfile, copytree, copy2, ignore_patterns
from setuptools import find_packages

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_call(['cmake', '--version'])
        except subprocess.CalledProcessError:
            raise RuntimeError("CMake must be installed to build the following extensions: " + ", ".join(e.name for e in self.extensions))

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPYTHON_EXECUTABLE=' + sys.executable
        ]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

        self.copy_extensions_to_source()


    def copy_extensions_to_source(self):
        build_temp = os.path.abspath(self.build_temp)
        ext_build_dir = os.path.abspath(self.build_lib)
        os.makedirs(ext_build_dir, exist_ok=True)

        # Copy torch_plugin_C.so
        src_file = os.path.join(build_temp, 'torch_plugin', 'torch_plugin_C.so')
        dst_file = os.path.join(ext_build_dir, 'torch_plugin', 'torch_plugin_C.so')

        if not os.path.isfile(src_file):
            raise RuntimeError(f"Expected shared library not found: {src_file}")

        # Create destination directory if it does not exist
        dst_dir = os.path.dirname(dst_file)
        os.makedirs(dst_dir, exist_ok=True)
        
        # Copy the shared library file
        copy2(src_file, dst_file)

        # Copy all files from build_temp/torch_plugin/lib to ext_output_dir/torch_plugin/lib
        src_lib_dir = os.path.join(build_temp, 'torch_plugin', 'lib')
        dst_lib_dir = os.path.join(ext_build_dir, 'torch_plugin', 'lib')

        if not os.path.exists(src_lib_dir):
            raise RuntimeError(f"Expected source directory not found: {src_lib_dir}")

        # Create destination directory if it does not exist
        os.makedirs(dst_lib_dir, exist_ok=True)
        
        # Copy all files from src_lib_dir to dst_lib_dir
        for root, _, files in os.walk(src_lib_dir):
            for file in files:
                src_file = os.path.join(root, file)
                rel_path = os.path.relpath(src_file, src_lib_dir)
                dst_file = os.path.join(dst_lib_dir, rel_path)
                dst_file_dir = os.path.dirname(dst_file)
                os.makedirs(dst_file_dir, exist_ok=True)
                copy2(src_file, dst_file)


setup(
    name='torch_plugin',
    version='0.0.1',
    author='jie.liu',
    author_email='lj057397@163.com',
    description='An example project dynamic modify pytorch',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/jie7397/pytorch_plugin',
    package_dir={'': 'src/python'},  # 指定包的源目录是 'src/python'
    packages=find_packages(where='src/python'),  # 从 'src/python' 目录中查找包
    package_data={
        'torch_plugin': ['torch_plugin_C.so', 'lib/*'],
    },
    ext_modules=[CMakeExtension('torch_plugin/torch_plugin_C')],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    install_requires=[
        'pybind11',
        'torch'
    ],
)
