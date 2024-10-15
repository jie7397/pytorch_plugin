cmake_minimum_required(VERSION 3.0)
project(torch_plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_subdirectory(src)

add_subdirectory(third_party/spdlog)