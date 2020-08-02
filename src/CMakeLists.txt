add_executable(
        http_cpp
        main.cpp
        http_request.cpp
        http_response.cpp
        http_server.cpp
        exception_socket.cpp
        exception_epoll.cpp
        exception_parser.cpp
)

target_include_directories(http_cpp PRIVATE ../inc)
target_link_libraries(http_cpp PRIVATE spdlog::spdlog)
