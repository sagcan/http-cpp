#ifndef HTTP_CPP_HTTP_SERVER_H
#define HTTP_CPP_HTTP_SERVER_H

#include <string>
#include "exception_socket.h"

namespace http {
class Server {
private:
    static constexpr int m_backlog_socket = 5;
    static constexpr int m_backlog_epoll = 10;
    static constexpr int m_buffer_size = 2048;

    int m_server_fd = -1;
    int m_epoll_fd = -1;

    int m_port;
    std::string m_directory;

    void init_socket();
    void init_epoll();
    void handle(const int client_fd) const;

public:
    Server(const int port, const std::string &directory);
    ~Server();
    void start() const;
};
}


#endif //HTTP_CPP_HTTP_SERVER_H