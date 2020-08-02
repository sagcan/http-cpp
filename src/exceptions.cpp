#include "../inc/exceptions.h"

HttpParserException::HttpParserException(const std::string &error) : m_error(error) {}

const char *HttpParserException::what() const noexcept {
    return m_error.c_str();
}


SocketException::SocketException(const std::string &error) : m_error(error) {}

const char *SocketException::what() const noexcept {
    return m_error.c_str();
}


EpollException::EpollException(const std::string &error) : m_error(error) {}

const char *EpollException::what() const noexcept {
    return m_error.c_str();
}
