#ifndef HTTP_CPP_EXCEPTION_PARSER_H
#define HTTP_CPP_EXCEPTION_PARSER_H


#include <string>

class HttpParserException : public std::exception {
private:
    std::string m_error;

public:
    HttpParserException(const std::string &error);
    const char *what() const noexcept;
};


#endif //HTTP_CPP_EXCEPTION_PARSER_H
