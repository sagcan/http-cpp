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

//    std::string req_chrome = "GET / HTTP/1.1\r\n"
//                             "Host: localhost:8080\r\n"
//                             "Connection: keep-alive\r\n"
//                             "Upgrade-Insecure-Requests: 1\r\n"
//                             "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36\r\n"
//                             "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
//                             "Sec-Fetch-Site: none\r\n"
//                             "Sec-Fetch-Mode: navigate\r\n"
//                             "Sec-Fetch-User: ?1\r\n"
//                             "Sec-Fetch-Dest: document\r\n"
//                             "Accept-Encoding: gzip, deflate, br\r\n"
//                             "Accept-Language: en,de-AT;q=0.9,de;q=0.8,en-US;q=0.7,de-DE;q=0.6\r\n\r\n";
//
//    std::string req_wget = "GET / HTTP/1.1\r\n"
//                           "User-Agent: Wget/1.20.3 (linux-gnu)\r\n"
//                           "Accept: */*\r\n"
//                           "Accept-Encoding: identity\r\n"
//                           "Host: localhost:8080\r\n"
//                           "Connection: Keep-Alive\r\n\r\n";
//
//    std::string req_firefox = "GET / HTTP/1.1\r\n"
//                              "Host: localhost:8080\r\n"
//                              "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0\r\n"
//                              "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
//                              "Accept-Language: en-US,en;q=0.5\r\n"
//                              "Accept-Encoding: gzip, deflate\r\n"
//                              "Connection: keep-alive\r\n"
//                              "Upgrade-Insecure-Requests: 1\r\n\r\n";
//
//    http::RequestHeader requestHeaderChrome;
//    http::RequestHeader requestHeaderFirefox;
//    http::RequestHeader requestHeaderWget;
//
//    std::cout << "=== CHROME ===" << std::endl;
//    std::cout << "RETURN VALUE: " << requestHeaderChrome.deserialize(req_chrome) << std::endl;
//    std::cout << static_cast<int>(requestHeaderChrome.get_method()) << std::endl;
//    std::cout << requestHeaderChrome.get_uri() << std::endl;
//    std::cout << requestHeaderChrome.get_header("Host") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Connection") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Upgrade-Insecure-Requests") << std::endl;
//    std::cout << requestHeaderChrome.get_header("User-Agent") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Accept") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Sec-Fetch-Site") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Sec-Fetch-Mode") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Sec-Fetch-User") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Sec-Fetch-Dest") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Accept-Encoding") << std::endl;
//    std::cout << requestHeaderChrome.get_header("Accept-Language") << std::endl;
//
//    std::cout << "\n=== FIREFOX ===" << std::endl;
//    std::cout << "RETURN VALUE: " << requestHeaderFirefox.deserialize(req_chrome) << std::endl;
//    std::cout << static_cast<int>(requestHeaderFirefox.get_method()) << std::endl;
//    std::cout << requestHeaderFirefox.get_uri() << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Host") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("User-Agent") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Accept") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Accept-Encoding") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Accept-Language") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Connection") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Upgrade-Insecure-Requests") << std::endl;
//
//    std::cout << "\n=== WGET ===" << std::endl;
//    std::cout << "RETURN VALUE: " << requestHeaderFirefox.deserialize(req_chrome) << std::endl;
//    std::cout << static_cast<int>(requestHeaderFirefox.get_method()) << std::endl;
//    std::cout << requestHeaderFirefox.get_uri() << std::endl;
//    std::cout << requestHeaderFirefox.get_header("User-Agent") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Accept") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Accept-Encoding") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Host") << std::endl;
//    std::cout << requestHeaderFirefox.get_header("Connection") << std::endl;

    return EXIT_SUCCESS;
}
