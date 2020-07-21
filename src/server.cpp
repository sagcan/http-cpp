#include <string>
#include <iostream>

#include <netinet/in.h>
#include <sys/epoll.h>

#include <cstring>
#include <unistd.h>

#include "server.h"

constexpr int BACKLOG_SOCKET = 5;
constexpr int BACKLOG_EPOLL = 10;
constexpr int BUFFER_SIZE = 2048;

static int server_init(const int port);
static int server_listen(const int server_fd, const std::string &dir);
static int server_handle(const int client_fd, const int epoll_fd, const std::string &dir);

int http::servert_start(int port, const std::string &dir) {
    int server_fd;

    if ((server_fd = server_init(port)) == -1) {
        return EXIT_FAILURE;
    }

    std::cout << "Directory: " << dir << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Server FD: " << server_fd << std::endl;

    // start main loop
    if (server_listen(server_fd, dir) == -1) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static int server_init(const int port) {
    int server_fd;
    struct sockaddr_in addr;

    /* we apply the SOCK_NONBLOCK flag here, because (e)poll-ing wouldn't be possible otherwise */
    if ((server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        std::cerr << "socket: " << std::strerror(errno) << std::endl;
        return -1;
    }

    addr.sin_family = AF_INET;          /* must be AF_INET according to ip(7) */
    addr.sin_port = htons(port);        /* don't forget the htons(), kids */
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1) {
        std::cerr << "bind: " << std::strerror(errno) << std::endl;
        return -1;
    }

    if (listen(server_fd, BACKLOG_SOCKET) == -1) {
        std::cerr << "listen: " << std::strerror(errno) << std::endl;
        return -1;
    }

    return server_fd;
}

static int server_listen(const int server_fd, const std::string &dir) {
    int epoll_fd;
    struct epoll_event ev;
    struct epoll_event events[BACKLOG_EPOLL];

    if ((epoll_fd = epoll_create1(0)) == -1) {
        std::cerr << "epoll_create1: " << std::strerror(errno) << std::endl;
        return -1;
    }

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        std::cerr << "epoll_ctl: " << std::strerror(errno) << std::endl;
        return -1;  // we can't proceed if the serve fd isn't in the epoll interest list
    }

    // TODO: find alternative approach, because this isn't really performant?
    //       (at least not for multiple connections) => epoll + threads?
    /* main event loop, poll for file descriptors;
     * depending on file descriptor (server vs client) accept new connections or handle to client requests */
    int nfds;
    for (;;) {
        if ((nfds = epoll_wait(epoll_fd, events, BACKLOG_EPOLL, -1)) == -1) {
            std::cerr << "epoll_wait: " << std::strerror(errno) << std::endl;
            exit(EXIT_FAILURE); // TODO: does this close any opened file descriptors automatically?
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                /* server: accept new connections and add them to the epoll interest list */
                int client_fd;
                if ((client_fd = accept4(server_fd, nullptr, nullptr, SOCK_NONBLOCK)) == -1) {
                    std::cerr << "accept4: " << std::strerror(errno) << std::endl;
                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    std::cerr << "epoll_ctl (EPOLL_CTL_ADD): " << std::strerror(errno) << std::endl;
                    continue;
                }

                std::cout << "new connection: " << client_fd << std::endl;
            } else {
                /* client: handle request and remove from epoll interest list if the connection is lost */
                server_handle(events[i].data.fd, epoll_fd, dir);
            }
        }
    }
}


static int server_handle(const int client_fd, const int epoll_fd, const std::string &dir) {
    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);    /* clean old values due to re-entering function / stack */

    if (read(client_fd, buf, BUFFER_SIZE) == 0) {
        std::cout << "Client lost; fd: " << client_fd << std::endl;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
        close(client_fd);
    } else {
//        std::string content(buf);
        std::cout << buf;
    }

    return 0;
}