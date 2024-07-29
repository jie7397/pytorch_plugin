
project(xtrans_demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(xtrans_op SHARED register_torch_op.cpp)
# set_target_properties(xtrans_op PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin)

# set_target_properties(XMLIRRuntime PROPERTIES
#   BUILD_WITH_INSTALL_RPATH OFF
#   BUILD_RPATH "\$ORIGIN"
#   INSTALL_RPATH "\$ORIGIN"
# )

# set_target_properties(torch_plugin PROPERTIES 
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin
#     INSTALL_RPATH "$ORIGIN"
#     BUILD_WITH_INSTALL_RPATH TRUE
# )
