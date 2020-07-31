#include "../inc/http_server.h"
#include "../inc/exception_epoll.h"
#include "../inc/http_request.h"
#include "../inc/http_response.h"
#include "../inc/exception_parser.h"

#include <string>
#include <iostream>

#include <netinet/in.h>
#include <sys/epoll.h>

#include <cstring>
#include <unistd.h>

http::Server::Server(int port, const std::string &directory) : m_port(port), m_directory(directory) {
    init_socket();  // init m_server_fd
    init_epoll();   // init m_epoll_fd
}

http::Server::~Server() {
    if (m_server_fd != -1) {
        close(m_server_fd);
    }

    if (m_epoll_fd != -1) {
        close(m_epoll_fd);
    }
}

void http::Server::init_socket() {
    struct sockaddr_in addr;

    /* we apply the SOCK_NONBLOCK flag here, because (e)poll-ing wouldn't be possible otherwise */
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        throw SocketException(std::strerror(errno));
    }

    addr.sin_family = AF_INET;          /* must be AF_INET according to ip(7) */
    addr.sin_port = htons(m_port);      /* don't forget the htons(), kids */
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_server_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        throw SocketException(std::strerror(errno));
    }

    if (listen(m_server_fd, m_backlog_socket) == -1) {
        throw SocketException(std::strerror(errno));
    }
}

void http::Server::init_epoll() {
    struct epoll_event ev;
    struct epoll_event events[m_backlog_epoll];

    if ((m_epoll_fd = epoll_create1(0)) == -1) {
        throw EpollException(std::strerror(errno));
    }

    ev.events = EPOLLIN;
    ev.data.fd = m_server_fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_server_fd, &ev) == -1) {
        throw EpollException(std::strerror(errno));
    }
}

void http::Server::start() const {
    struct epoll_event ev;
    struct epoll_event events[m_backlog_epoll];

    int nfds;
    for (;;) {
        if ((nfds = epoll_wait(m_epoll_fd, events, m_backlog_epoll, -1)) == -1) {
            std::cerr << "epoll_wait: " << std::strerror(errno) << std::endl;
            throw EpollException(std::strerror(errno)); // TODO: data-structure that holds all active fd and cleans them up on throw (= destructor)
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == m_server_fd) {
                /* server: accept new connections and add them to the epoll interest list */
                int client_fd;
                if ((client_fd = accept4(m_server_fd, nullptr, nullptr, SOCK_NONBLOCK)) == -1) {
                    std::cerr << "accept4: " << std::strerror(errno) << std::endl;
                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    std::cerr << "epoll_ctl (EPOLL_CTL_ADD): " << std::strerror(errno) << std::endl;
                    continue;
                }

                std::cout << "new connection: " << client_fd << std::endl;
            } else {
                /* client: handle request and remove from epoll interest list if the connection is lost */
                handle(events[i].data.fd);
            }
        }
    }
}

void http::Server::handle(const int client_fd) const {
    char buf[m_buffer_size];
    memset(buf, 0, m_buffer_size);    /* clean old values due to re-entering function / stack */

    if (read(client_fd, buf, m_buffer_size) == 0) {
        std::cout << "Client lost; fd: " << client_fd << std::endl;
        epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
        close(client_fd);

        return;
    }

    try {
        http::RequestHeader requestHeader(buf);
        if (requestHeader.get_method() != http::constants::Method::GET) {
            // we only support GET methods for now, so let's return a 405 error
            // see https://tools.ietf.org/html/rfc7231#section-6.5.5

            const char *response = "HTTP/1.1 405 Method not Allowed\r\nAllow: GET\r\n\r\n";
            write(client_fd, response, std::strlen(response));
        } else {
            http::ResponseHeader responseHeader(m_directory);
            std::string response = responseHeader.serialize(requestHeader);
            write(client_fd, response.c_str(), response.size());
        }
    } catch (HttpParserException &e) {
        std::cerr << e.what() << std::endl;
    }
}

