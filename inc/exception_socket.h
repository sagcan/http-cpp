#ifndef HTTP_CPP_EXCEPTION_SOCKET_H
#define HTTP_CPP_EXCEPTION_SOCKET_H


#include <exception>
#include <string>

class SocketException : public std::exception {
private:
    std::string m_error;

public:
    SocketException(const std::string &error);
    const char *what() const noexcept;
};


#endif //HTTP_CPP_EXCEPTION_SOCKET_H
