
project(xtrans_demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(register_torch_op SHARED register_torch_op.cpp)

set_target_properties(register_torch_op PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin/lib
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)
