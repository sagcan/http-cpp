#include "../inc/exception_parser.h"

HttpParserException::HttpParserException(const std::string &error) : m_error(error) {}

const char *HttpParserException::what() const noexcept {
    return m_error.c_str();
}
