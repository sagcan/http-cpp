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

public:
    RequestHeader();
    RequestHeader(const std::string &content);
    RequestHeader(const std::string &content, const std::string &root_file_name);
    constants::Method get_method() const;
    std::string get_uri() const;
    std::string get_header(const std::string &key) const;
    int deserialize(const std::string &content);
};
}

#endif //HTTP_CPP_HTTP_REQUEST_H
