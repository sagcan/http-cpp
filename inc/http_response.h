#ifndef HTTP_CPP_HTTP_RESPONSE_H
#define HTTP_CPP_HTTP_RESPONSE_H

#include <string>
#include <map>
#include "http_request.h"

namespace http {
class ResponseHeader {
private:
    std::string m_dir_path;
    std::map<std::string, std::string> m_misc_headers;

public:
    ResponseHeader(const std::string &dir_path);
    void add_header(const std::string &key, const std::string &value);
    std::string serialize(const RequestHeader &req_header);
};
}


#endif //HTTP_CPP_HTTP_RESPONSE_H
