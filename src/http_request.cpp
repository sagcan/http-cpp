#include "../inc/http_request.h"

http::RequestHeader::RequestHeader()    // TODO: remove definition here; default should be with content file
        : m_root_file_name("index.html") {};

http::RequestHeader::RequestHeader(const std::string &content)
        : m_root_file_name("index.html") {
    // TODO: implement constructor with content argument; call deserialize here?
}

http::RequestHeader::RequestHeader(const std::string &content, const std::string &root_file_name)
        : m_root_file_name(root_file_name) {
    // TODO: implement constructor with content argument; call deserialize here?
}

http::constants::Method http::RequestHeader::get_method() const {
    return m_method;
}

std::string http::RequestHeader::get_uri() const {
    if (m_uri == "/") {
        return m_root_file_name;
    }

    return m_uri.substr(1, m_uri.size()); // return URI without leading slash
}

std::string http::RequestHeader::get_header(const std::string &key) const {
    auto res = m_misc_headers.find(key);

    if (res == m_misc_headers.end()) {
        // TODO: throw exception?
        return "";
    }

    return res->second;
}

int http::RequestHeader::deserialize(const std::string &content) {
    size_t pos_curr = 0;
    size_t pos_prev = 0;

    // HTTP-Method
    // 1. advance to first whitespace
    // 2. extract substring
    // 3. check if substring is a valid HTTP-Method by finding that element in our std::map
    pos_curr = content.find_first_of(' ', pos_prev);
    if (pos_curr == std::string::npos) {
        return -1;
    }

    auto res = constants::method_map.find(content.substr(pos_prev, pos_curr - pos_prev));
    if (res == constants::method_map.end()) {
        return -1;
    }
    m_method = res->second;
    pos_prev = ++pos_curr;  // advance by one character (currently sitting on whitespace)

    // URI
    // 1. advance to first whitespace
    // 2. extract substring
    pos_curr = content.find_first_of(' ', pos_prev);
    if (pos_curr == std::string::npos) {
        return -1;
    }
    m_uri = content.substr(pos_prev, pos_curr - pos_prev);
    pos_prev = ++pos_curr;  // advance by one character (currently sitting on whitespace)

    // HTTP Version
    // 1. advance to CRLF sequence (= "\r\n")
    // 2. extract substring
    // 3. check if HTTP version is 1.1
    pos_curr = content.find_first_of(constants::LINE_ENDING, pos_prev);
    if (pos_curr == std::string::npos || content.substr(pos_prev, (pos_curr - pos_prev) - 1) == constants::HTTP_VERSION) {
        return -1;
    }
    pos_prev = ++(++pos_curr);  // advance to next line (currently sitting on CRLF sequence -> '\r\n')


    // Misc Headers
    // 1. advance to first colon
    // 2. extract substring (= key)
    // 3. advance to CRLF sequence
    // 4. extract substring (= value)
    // 5. insert key and value into std::map
    std::string key;
    std::string value;
    for (;;) {
        // key
        pos_curr = content.find_first_of(':', pos_prev);
        if (pos_curr == std::string::npos) {
            break;
        }
        key = content.substr(pos_prev, pos_curr - pos_prev);
        pos_prev = ++(++pos_curr); // advance by two characters (currently sitting on a colon followed by whitespace)

        // value
        pos_curr = content.find_first_of(constants::LINE_ENDING, pos_prev);
        if (pos_curr == std::string::npos) {
            break;
        }
        value = content.substr(pos_prev, pos_curr - pos_prev);
        m_misc_headers.insert(std::make_pair(key, value));
        pos_prev = ++(++pos_curr); // advance to next line (currently sitting on CRLF sequence -> '\r\n')
    }

    return 0;
}


