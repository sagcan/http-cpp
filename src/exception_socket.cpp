#include "../inc/exception_socket.h"

SocketException::SocketException(const std::string &error) : m_error(error) {}

const char *SocketException::what() const noexcept {
    return m_error.c_str();
}
