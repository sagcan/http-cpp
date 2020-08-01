#include "../inc/http_server.h"
#include "../inc/exception_epoll.h"
#include "../inc/http_request.h"
#include "../inc/http_response.h"
#include "../inc/exception_parser.h"

#include <string>

#include <netinet/in.h>
#include <sys/epoll.h>

#include <cstring>
#include <unistd.h>
#include <spdlog/spdlog.h>

http::Server::Server(int port, const std::string &directory) : m_port(port), m_directory(directory) {
    // m_server_fd and m_epoll_fd will be initialized inside init_socket() and init_epoll() respectively
    init_socket();
    init_epoll();
}

http::Server::~Server() {
    spdlog::debug("Executing destructor...");
    if (m_server_fd != -1) {
        close(m_server_fd);
    }

    if (m_epoll_fd != -1) {
        close(m_epoll_fd);
    }
}

/**
 * Create a socket, retrieve it's file descriptor and finally initialize the member variable m_server_fd.
 */
void http::Server::init_socket() {
    struct sockaddr_in addr;

    /* we apply the SOCK_NONBLOCK flag here, because (e)poll-ing wouldn't be possible otherwise */
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        spdlog::error("socket: {}", std::strerror(errno));
        throw SocketException(std::strerror(errno));
    }

    addr.sin_family = AF_INET;          /* must be AF_INET according to ip(7) */
    addr.sin_port = htons(m_port);      /* don't forget the htons(), kids */
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_server_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        spdlog::error("bind: {}", std::strerror(errno));
        throw SocketException(std::strerror(errno));
    }

    if (listen(m_server_fd, m_backlog_socket) == -1) {
        spdlog::error("listen: {}", std::strerror(errno));
        throw SocketException(std::strerror(errno));
    }
}

/**
 * Create an epoll instance, retrieve it's file descriptor and finally initialize the member variable m_epoll_fd.
 */
void http::Server::init_epoll() {
    struct epoll_event ev;
    struct epoll_event events[m_backlog_epoll];

    if ((m_epoll_fd = epoll_create1(0)) == -1) {
        spdlog::error("epoll_create1: {}", std::strerror(errno));
        throw EpollException(std::strerror(errno));
    }

    ev.events = EPOLLIN;
    ev.data.fd = m_server_fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_server_fd, &ev) == -1) {
        spdlog::error("epoll_ctl: {}", std::strerror(errno));
        throw EpollException(std::strerror(errno));
    }
}

/**
 * Start the server main loop and serve request upon epoll events. The main loop itself is "sleeping" until an epoll
 * event happens, so no busy waiting whatsover.
 */
void http::Server::start() const {
    spdlog::info("Main loop started");

    struct epoll_event ev;
    struct epoll_event events[m_backlog_epoll];

    int nfds;
    for (;;) {
        if ((nfds = epoll_wait(m_epoll_fd, events, m_backlog_epoll, -1)) == -1) {
            spdlog::error("epoll_wait: {}", strerror(errno));
            throw EpollException(std::strerror(errno)); // TODO: data-structure that holds all active fd and cleans them up on throw (= destructor)
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == m_server_fd) {
                // server event => accept new connection and add it to the epoll interest list
                int client_fd;
                if ((client_fd = accept4(m_server_fd, nullptr, nullptr, SOCK_NONBLOCK)) == -1) {
                    spdlog::warn("accept4: {}", strerror(errno));
                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    spdlog::warn("epoll_ctl (EPOLL_CTL_ADD): {}", strerror(errno));
                    continue;
                }

                spdlog::debug("New connection with file descriptor {}", client_fd); // TODO: more client info
            } else {
                // client event => handle request and remove file descriptor from epoll interest list if need be
                handle(events[i].data.fd);
            }
        }
    }
}

/**
 * Retrieve a HTTP request / message from a client, then parse and create a response accordingly.
 * Also close the connection to the client and remove it from the epoll interest list if need be.
 *
 * @param client_fd the file descriptor of the to be served client
 */
void http::Server::handle(const int client_fd) const {
    char buf[m_buffer_size];
    memset(buf, 0, m_buffer_size);    /* clean old values due to re-entering function / stack */

    if (read(client_fd, buf, m_buffer_size) == 0) {
        // EOF => close connection
        spdlog::debug("Lost connection with file descriptor {}", client_fd); // TODO: more client info
        epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
        close(client_fd);

        return;
    }

    try {
        // parse request and create response
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
        // the RequestHeader class doesn't have any "serious" exception so we just use the messages just for debug
        // purposes inside the catch-block; exception can only occur if the request format itself isn't valid
        spdlog::debug("HttpParserException: {}", e.what());
    }
}

