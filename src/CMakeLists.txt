
project(torch_plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(util)

# for op register demo
include_directories(op_register)
add_subdirectory(op_register)

# for op deregister demo
include_directories(op_deregister)

# for op plt hook
include_directories(plt_hook)

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


list(APPEND CMAKE_PREFIX_PATH ${PybindOutput})

# Find pybind11
find_package(pybind11 REQUIRED)

# Find Torch
find_package(Torch REQUIRED)
find_library(TORCH_PYTHON_LIBRARY torch_python PATH "${TORCH_PATH}/lib")

file(GLOB_RECURSE OP_DEREGISTER_HEADERS  "./op_deregister/*.h")
file(GLOB_RECURSE OP_DEREGISTER_SRCS "./op_deregister/*.cpp")

file(GLOB_RECURSE PLTHOOK_HEADERS  "./plt_hook/*.h")
file(GLOB_RECURSE PLTHOOK_SRCS "./plt_hook/*.cpp")

add_library(torch_plugin SHARED ${OP_DEREGISTER_SRCS} ${PLTHOOK_SRCS} util/log.cpp)

target_include_directories(torch_plugin PRIVATE ${spdlog_SOURCE_DIR}/include)

target_link_libraries(torch_plugin PRIVATE ${TORCH_LIBRARIES} ${TORCH_PYTHON_LIBRARY})

set_target_properties(torch_plugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin/lib
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)

add_library(torch_plugin_C MODULE py_bind/bind.cc)

target_include_directories(torch_plugin_C PRIVATE ${spdlog_SOURCE_DIR}/include)

target_link_libraries(torch_plugin_C PRIVATE register_torch_op torch_plugin pybind11::module ${TORCH_LIBRARIES} ${TORCH_PYTHON_LIBRARY})
set_target_properties(torch_plugin_C PROPERTIES PREFIX "")

add_dependencies(torch_plugin_C torch_plugin)
add_dependencies(torch_plugin_C register_torch_op)

set_target_properties(torch_plugin_C PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin
    BUILD_RPATH "\$ORIGIN/../build:\$ORIGIN"
    INSTALL_RPATH "\$ORIGIN/../build:\$ORIGIN"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)