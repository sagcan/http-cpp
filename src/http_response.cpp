#include <fstream>
#include <filesystem>
#include "../inc/http_response.h"

http::ResponseHeader::ResponseHeader(const std::string &dir_path)
        : m_dir_path(dir_path) {}
/**
 * Add miscellaneous header into the member map
 *
 * @param key The header key (i.e. Content-Type)
 * @param value The header value (i.e. text/html; charset=UTF-8)
 */
void http::ResponseHeader::add_header(const std::string &key, const std::string &value) {
    m_misc_headers.insert(std::make_pair(key, value));
}

/**
 * Serialize a response out of member variables like so:
 * 1. Serialize the Status-Line (HTTP-Version Status-Code Reason-Phrase)
 * 2. Serialize the Headers (i.e. Content-Type: text/html; charset=UTF-8) by iterating over all available ones inside
 * the map (m_misc_headers)
 * 3. Append file-content (if needed) onto serialized message
 * 4. Return String
 *
 * @param req_header Headers of the initial request
 * @return a serialized HTTP response
 */
std::string http::ResponseHeader::serialize(const RequestHeader &req_header) const {
    // 1. open file (return empty string if not works; eventually replace with exception)
    // 2. create response line
    // 3. iterate over headers and add them
    // 4. add content of opened file
    // 5. return file
    std::string response;
    std::string file_path = std::string(m_dir_path + "/" + req_header.get_uri());

    std::ifstream ifs(file_path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    if (ifs.good()) {
        auto status_code_pair = constants::statuscode_map.find(constants::OK);  // we can assume that OK exists in map

        // response line
        response.append(constants::HTTP_VERSION);
        response.append(" ");
        response.append(std::to_string(status_code_pair->first));
        response.append(" ");
        response.append(status_code_pair->second);
        response.append(constants::LINE_ENDING);

        // headers
        if (!m_misc_headers.empty()) {
            for (auto const &[key, val] : m_misc_headers) {
                response.append(key);
                response.append(": ");
                response.append(val);
                response.append(constants::LINE_ENDING);
            }
        }

        // content
        response.append("Content-Length: ");
        response.append(std::to_string(content.size()));
        response.append(constants::LINE_ENDING);    // two CRLF sequences needed to indicate start of content
        response.append(constants::LINE_ENDING);
        response.append(content);
    } else if (!std::filesystem::exists(file_path)) {
        // 404 error
        response.append(constants::HTTP_VERSION);
        response.append(" 404 Not Found");
        response.append(constants::LINE_ENDING);
        response.append("Content-Length: ");
        response.append(std::to_string(constants::NOT_FOUND_ERROR_MESSAGE.size()));
        response.append(constants::LINE_ENDING);
        response.append(constants::LINE_ENDING);
        response.append(constants::NOT_FOUND_ERROR_MESSAGE);
    } else {
        // 500 error
        response.append(constants::HTTP_VERSION);
        response.append(" 500 Internal Server Error");
        response.append(constants::LINE_ENDING);
        response.append("Content-Length: ");
        response.append(std::to_string(constants::INTERNAL_SERVER_ERROR_MESSAGE.size()));
        response.append(constants::LINE_ENDING);
        response.append(constants::LINE_ENDING);
        response.append(constants::INTERNAL_SERVER_ERROR_MESSAGE);
    }

    return response;
}

