cmake_minimum_required(VERSION 3.16)
project(http_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(http_cpp src/main.cpp src/server.cpp inc/server.h inc/http_constants.h src/http_request.cpp inc/http_request.h src/http_response.cpp inc/http_response.h)