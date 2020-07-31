#ifndef HTTP_CPP_EXCEPTION_EPOLL_H
#define HTTP_CPP_EXCEPTION_EPOLL_H


#include <exception>
#include <string>

class EpollException : public std::exception {
private:
    std::string m_error;

public:
    EpollException(const std::string &error);
    const char *what() const noexcept;
};


#endif //HTTP_CPP_EXCEPTION_EPOLL_H
