#ifndef HTTP_CPP_HTTP_HEADER_H
#define HTTP_CPP_HTTP_HEADER_H

#include "http_method.h"

#include <map>

namespace http {
class Header {
private:
    std::map<std::string, std::string> m_misc_headers;
    std::string m_uri;
    std::string m_method;

public:
    std::string get_value(const std::string &key);
    std::string get_uri();
    http::Methods get_method();
    int deserialize(const std::string &content);
};
}


#endif //HTTP_CPP_HTTP_HEADER_H
