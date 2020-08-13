#include <iostream>
#include <climits>
#include <filesystem>
#include <spdlog/spdlog.h>

#include "../inc/http_server.h"
#include "../inc/exceptions.h"

#define HTTP_CPP_DEBUG

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " [directory] [port]\n";
        return EXIT_FAILURE;
    }

    std::string dir = argv[1];
    int port = std::stoi(argv[2]);

    if (port <= 0 || port > USHRT_MAX) {
        std::cerr << "Invalid port value\n";
        return EXIT_FAILURE;
    }

    if (!std::filesystem::is_directory(std::filesystem::path(dir))) {
        std::cerr << "Invalid directory\n";
        return EXIT_FAILURE;
    }

#ifdef HTTP_CPP_DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif
    spdlog::set_pattern("[%d.%m.%Y %H:%M:%S] [%^%l%$] %v");

    try {
        http::Server server(port, dir);
        server.start();
    } catch (SocketException &e) {
        std::cerr << e.what() << std::endl;
    } catch (EpollException &e) {
        std::cerr << e.what() << std::endl;
    }


    return EXIT_SUCCESS;
}
