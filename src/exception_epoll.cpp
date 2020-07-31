#include "../inc/exception_epoll.h"

EpollException::EpollException(const std::string &error) : m_error(error) {}

const char *EpollException::what() const noexcept {
    return m_error.c_str();
}
