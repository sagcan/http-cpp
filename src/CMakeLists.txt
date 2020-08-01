add_executable(
        http_cpp
        main.cpp
                                ../inc/http_constants.h
        http_request.cpp        ../inc/http_request.h
        http_response.cpp       ../inc/http_response.h
        http_server.cpp         ../inc/http_server.h
        exception_socket.cpp    ../inc/exception_socket.h
        exception_epoll.cpp     ../inc/exception_epoll.h
        exception_parser.cpp    ../inc/exception_parser.h
)

target_link_libraries(http_cpp PRIVATE spdlog::spdlog)
