#include <vector>
#include "http_header.h"

std::string http::Header::get_value(const std::string &key) {
    auto val = m_misc_headers.find(key);
    if (val == m_misc_headers.end()) {
        return std::string();
    }

    return val->second;
}

std::string http::Header::get_uri() {
    return m_uri;
}

http::Methods http::Header::get_method() {
    for (int i = 0; i < HTTP_METHODS.size(); ++i) {
        if (m_method == HTTP_METHODS[i]) {
            return static_cast<http::Methods>(i);
        }
    }

    return http::Methods::UNKOWN;
}

int http::Header::deserialize(const std::string &content) {
    size_t content_len = content.size();
    size_t pos_prev = 0;
    size_t pos_curr = 0;

    //
    // REQUEST LINE
    //

    // method
    pos_curr = content.find_first_of(' ', pos_curr);
    if (pos_curr == std::string::npos) {
        return -1;
    }

    m_method = content.substr(pos_prev, pos_curr - pos_prev);
    if (get_method() == http::Methods::UNKOWN) {
        return -1;
    }

    // uri
    pos_prev = ++pos_curr;
    pos_curr = content.find_first_of(' ', pos_curr);
    if (pos_curr == std::string::npos) {
        return -1;
    }

    m_uri = content.substr(pos_prev, pos_curr - pos_prev);

    // method
    pos_prev = ++pos_curr;
    pos_curr = content.find_first_of('\r', pos_curr);
    if (pos_curr == std::string::npos) {
        return -1;
    }

    if (content.substr(pos_prev, pos_curr - pos_prev) != "HTTP/1.1") {
        // we support only HTTP 1.1 (for now)
        return -1;
    }

    //
    // MISC HEADERS
    //
    std::string key;
    std::string value;
    while (true) {
        ++(++pos_curr); // our cursor is currently at a '\r', increasing once get's us to '\n', increasing it again to the next line
        pos_prev = pos_curr;
        pos_curr = content.find_first_of(':', pos_curr);
        if (pos_curr == std::string::npos) {
            break;
        }

        key = content.substr(pos_prev, pos_curr - pos_prev);

        pos_prev = ++(++pos_curr);
        pos_curr = content.find_first_of('\r', pos_curr);
        if (pos_curr == std::string::npos) {
            break;
        }

        value = content.substr(pos_prev, pos_curr - pos_prev);

        m_misc_headers.insert(std::make_pair(key, value));
    }

    return 0;
}
