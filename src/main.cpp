#include <map>
#include <iostream>
#include <climits>
#include <filesystem>

#include "server.h"

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

    if (std::filesystem::is_directory(std::filesystem::path(dir)) == false) {
        std::cerr << "Invalid directory\n";
        return EXIT_FAILURE;
    }


    http::servert_start(port, dir);
    return EXIT_SUCCESS;
}
