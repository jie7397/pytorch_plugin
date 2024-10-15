
project(xtrans_demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB REGISTER_TORCH_OP_SOURCES "./*.cpp")

add_library(register_torch_op SHARED ${REGISTER_TORCH_OP_SOURCES} )

target_link_libraries(register_torch_op PRIVATE spdlog::spdlog_header_only)

set_target_properties(register_torch_op PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/torch_plugin/lib
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
)
