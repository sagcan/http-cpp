//
// Created by volkan on 24.07.20.
//

#ifndef HTTP_CPP_HTTP_RESPONSE_H
#define HTTP_CPP_HTTP_RESPONSE_H

#include <utility>
#include <string>
#include <map>

#include "http_method.h"

namespace http {
class ResponseHeader {
private:
    std::string m_http_version = "HTTP/1.1";
    std::pair <int, std::string> m_status_code;
    std::map<std::string, std::string> m_misc_headers;

public:
    ResponseHeader() = default;
    std::string serialize(const std::string &content);
    void add_header_element(const std::string &key, const std::string &value);
    void add_status(http::StatusCode code);
};
}


#endif //HTTP_CPP_HTTP_RESPONSE_H
