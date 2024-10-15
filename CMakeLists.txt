cmake_minimum_required(VERSION 3.5)
project(torch_plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

execute_process(
    COMMAND python ${CMAKE_SOURCE_DIR}/scripts/set_torch.py install
    OUTPUT_VARIABLE output
    RESULT_VARIABLE result
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT result EQUAL "0")
    message(FATAL_ERROR "Python script failed with result code: ${result}")
endif()

add_custom_target(revert_private_changes ALL
    COMMAND bash -c "python ${CMAKE_SOURCE_DIR}/scripts/set_torch.py uninstall"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS torch_plugin_C
)

add_subdirectory(src)

add_subdirectory(third_party/spdlog)