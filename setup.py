import os
import re
import subprocess
import sys
import platform
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
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

setup(
    name='torch_plugin',
    version='0.0.1',
    author='jie.liu',
    author_email='lj057397@163.com',
    description='An example project linking C++ and PyTorch using pybind11.',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/jie7397/pytorch_plugin',
    packages=find_packages(),
    ext_modules=[CMakeExtension('torch_plugin_C')],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    install_requires=[
        'pybind11',
        'torch'
    ],
)