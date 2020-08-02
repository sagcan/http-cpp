#ifndef HTTP_CPP_EXCEPTIONS_H
#define HTTP_CPP_EXCEPTIONS_H


#include <string>

class SocketException : public std::exception {
private:
    std::string m_error;

public:
    SocketException(const std::string &error);
    const char *what() const noexcept;
};



class EpollException : public std::exception {
private:
    std::string m_error;

public:
    EpollException(const std::string &error);
    const char *what() const noexcept;
};



class HttpParserException : public std::exception {
private:
    std::string m_error;

public:
    HttpParserException(const std::string &error);
    const char *what() const noexcept;
};



#endif //HTTP_CPP_EXCEPTIONS_H
