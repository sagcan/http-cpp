#ifndef HTTP_CPP_HTTP_REQUEST_H
#define HTTP_CPP_HTTP_REQUEST_H

#include "http_constants.h"

namespace http {
class RequestHeader {
private:
    std::map<std::string, std::string> m_misc_headers;
    std::string m_uri;
    constants::Method m_method;

    std::string m_root_file_name; // root file in case URI equals '/' (i.e. '/' = index.html)

    void deserialize(const std::string &content);

public:
    RequestHeader(const std::string &content);
    RequestHeader(const std::string &content, const std::string &root_file_name);

    const constants::Method get_method() const;
    const std::string &get_uri() const;
    const std::string &get_header(const std::string &key) const;
};
}

#endif //HTTP_CPP_HTTP_REQUEST_H
