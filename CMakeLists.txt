cmake_minimum_required(VERSION 3.0)
project(torch_plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)


find_package(PythonInterp 3 REQUIRED)

set(TORCH_PATH "
import os
import torch

torch_path = os.path.dirname(torch.__file__)
print(torch_path)
")

execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "${TORCH_PATH}"
    OUTPUT_VARIABLE TorchOutput
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

list(APPEND CMAKE_PREFIX_PATH ${TorchOutput})

set(PYBIND_PATH "
import pybind11
print(pybind11.get_cmake_dir())
")


execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "${PYBIND_PATH}"
    OUTPUT_VARIABLE PybindOutput
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# for trans register
include_directories(xtrans_demo)

list(APPEND CMAKE_PREFIX_PATH ${PybindOutput})

# Find pybind11
find_package(pybind11 REQUIRED)

# Find Torch
find_package(Torch REQUIRED)

add_subdirectory(xtrans_demo)

add_library(torch_plugin_C MODULE bind.cc)


target_link_libraries(torch_plugin_C PRIVATE xtrans_op pybind11::module ${TORCH_LIBRARIES})
set_target_properties(torch_plugin_C PROPERTIES PREFIX "")
# set_target_properties(torch_plugin_C PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin)



# set_target_properties(torch_plugin_C PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" SUFFIX "${PYTHON_MODULE_EXTENSION}")