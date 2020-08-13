cmake_minimum_required(VERSION 3.16)
project(http_cpp)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)
