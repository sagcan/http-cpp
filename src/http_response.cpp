#include "http_response.h"
#include "http_method.h"

constexpr size_t BUFFER_SIZE = 4096;
constexpr std::string_view LINE_ENDING = "\r\n";

void http::ResponseHeader::add_header_element(const std::string &key, const std::string &value) {
    m_misc_headers.insert({key, value});
}

std::string http::ResponseHeader::serialize(const std::string &content) {
    std::string response;
    response.reserve(BUFFER_SIZE);

    response.append(m_http_version);
    response.append(" ");
    response.append(std::to_string(m_status_code.first));
    response.append(" ");
    response.append(m_status_code.second);
    response.append(LINE_ENDING);

    // TODO: exception can be thrown, see docs
    for (const auto &it : m_misc_headers) {
        response.append(it.first);
        response.append(": ");
        response.append(it.second);
        response.append(LINE_ENDING);
    }

    response.append("Content-Length: ");
    response.append(std::to_string(content.length()));
    response.append(LINE_ENDING);
    response.append(LINE_ENDING);

    response.append(content);

    return response;
}

void http::ResponseHeader::add_status(http::StatusCode code) {
    auto val = http::status_code_maps.find(code);

    if (val == http::status_code_maps.end()) {
        m_status_code = std::make_pair(500, "Internal Server Error");
    } else {
        m_status_code = std::make_pair(val->first, val->second);
    }
}
