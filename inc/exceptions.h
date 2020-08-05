#ifndef HTTP_CPP_EXCEPTIONS_H
#define HTTP_CPP_EXCEPTIONS_H

#include <string>
#include <stdexcept>

class SocketException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class EpollException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class HttpParserException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

#endif //HTTP_CPP_EXCEPTIONS_H
