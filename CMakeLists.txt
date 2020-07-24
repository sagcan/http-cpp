cmake_minimum_required(VERSION 3.16)
project(http_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(http_cpp src/main.cpp src/server.cpp src/server.h src/http_request.cpp src/http_request.h src/http_method.h src/http_response.cpp src/http_response.h)