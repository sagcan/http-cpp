cmake_minimum_required(VERSION 3.16)
project(http_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        http_cpp
        src/main.cpp
        inc/http_constants.h
        src/http_request.cpp inc/http_request.h
        src/http_response.cpp inc/http_response.h
        src/http_server.cpp inc/http_server.h
        src/exception_socket.cpp inc/exception_socket.h
        src/exception_epoll.cpp inc/exception_epoll.h
        src/exception_parser.cpp inc/exception_parser.h)