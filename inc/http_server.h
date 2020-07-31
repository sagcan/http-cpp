#ifndef HTTP_CPP_HTTP_SERVER_H
#define HTTP_CPP_HTTP_SERVER_H

#include <string>

namespace http {
class Server {
private:
    static constexpr int m_backlog_socket = 5;
    static constexpr int m_backlog_epoll = 10;
    static constexpr int m_buffer_size = 2048;

    int m_server_fd;
    int m_epoll_fd;

    int m_port;
    std::string m_directory;

    void init_socket();
    void init_epoll();
    void handle(const int client_fd);

public:
    Server(const int port, const std::string &directory);
    void start();

};
}


#endif //HTTP_CPP_HTTP_SERVER_H