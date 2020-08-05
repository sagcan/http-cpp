add_executable(
        http_cpp
        main.cpp
        http_request.cpp
        http_response.cpp
        http_server.cpp
)

target_include_directories(http_cpp PRIVATE ../inc)
target_link_libraries(http_cpp PRIVATE spdlog::spdlog)
