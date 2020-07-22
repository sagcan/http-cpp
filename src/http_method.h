#ifndef HTTP_CPP_HTTP_METHOD_H
#define HTTP_CPP_HTTP_METHOD_H

#include <vector>
#include <string>

namespace http {

// Note: order of vector and enum elements has to be same (i.e. GET must be first element in vector and enum)
const std::vector<std::string> HTTP_METHODS = {"GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT", "UNDEFINED"};
enum class Methods {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    UNKOWN      // DO NOT INCLUDE IN VECTOR; return value in case no valid HTTP method was found
};



}   // namespace http
#endif //HTTP_CPP_HTTP_METHOD_H
