#include <map>
#include <iostream>
#include <climits>
#include <filesystem>

#include "server.h"

static bool are_valid_args(const int port, const std::string &dir);

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " [directory] [port]\n";
        return EXIT_FAILURE;
    }

    std::string dir = argv[1];
    int port = std::stoi(argv[2]);

    if (are_valid_args(port, dir) == false) {
        return EXIT_FAILURE;
    }

    http::servert_start(port, dir);

    return EXIT_SUCCESS;
}

static bool are_valid_args(const int port, const std::string &dir) {
    if (port < 0 || port > USHRT_MAX) { // TODO: check if port == 0 valid
        std::cerr << "Invalid port value\n";
        return false;
    }

    if (std::filesystem::is_directory(std::filesystem::path(dir)) == false) {
        std::cerr << "Invalid directory\n";
        return false;
    }

    return true;
}
