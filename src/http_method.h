#ifndef HTTP_CPP_HTTP_METHOD_H
#define HTTP_CPP_HTTP_METHOD_H

#include <vector>
#include <string>
#include <map>

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


enum StatusCode {
    CONTINUE                          = 100,
    SWITCHING_PROTOCOLS               = 101,
    OK                                = 200,
    CREATED                           = 201,
    ACCEPTED                          = 202,
    NON_AUTHORITATIVE_INFORMATION     = 203,
    NO_CONTENT                        = 204,
    RESET_CONTENT                     = 205,
    PARTIAL_CONTENT                   = 206,
    MULTIPLE_CHOICES                  = 300,
    MOVED_PERMANENTLY                 = 301,
    FOUND                             = 302,
    SEE_OTHER                         = 303,
    NOT_MODIFIED                      = 304,
    USE_PROXY                         = 305,
    TEMPORARY_REDIRECT                = 307,
    BAD_REQUEST                       = 400,
    UNAUTHORIZED                      = 401,
    PAYMENT_REQUIRED                  = 402,
    FORBIDDEN                         = 403,
    NOT_FOUND                         = 404,
    METHOD_NOT_ALLOWED                = 405,
    NOT_ACCEPTABLE                    = 406,
    PROXY_AUTHENTICATION_REQUIRED     = 407,
    REQUEST_TIME_OUT                  = 408,
    CONFLICT                          = 409,
    GONE                              = 410,
    LENGTH_REQUIRED                   = 411,
    PRECONDITION_FAILED               = 412,
    REQUEST_ENTITY_TOO_LARGE          = 413,
    REQUEST_URI_TOO_LARGE             = 414,
    UNSUPPORTED_MEDIA_TYPE            = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE   = 416,
    EXPECTATION_FAILED                = 417,
    INTERNAL_SERVER_ERROR             = 500,
    NOT_IMPLEMENTED                   = 501,
    BAD_GATEWAY                       = 502,
    SERVICE_UNAVAILABLE               = 503,
    GATEWAY_TIME_OUT                  = 504,
    HTTP_VERSION_NOT_SUPPORTED        = 505
};

// TODO: without const linker error???????????
const std::map<int, std::string> status_code_maps = {
        {100, "Continue"},
        {101, "Switching Protocols"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {307, "Temporary Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Time-out"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Request Entity Too Large"},
        {414, "Request-URI Too Large"},
        {415, "Unsupported Media Type"},
        {416, "Requested range not satisfiable"},
        {417, "Expectation Failed"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Time-out"},
        {505, "HTTP Version not supported"}
};



}   // namespace http
#endif //HTTP_CPP_HTTP_METHOD_H
