#include <fstream>
#include <filesystem>
#include "../inc/http_response.h"
#include "../inc/http_request.h"
#include "../inc/http_constants.h"

http::ResponseHeader::ResponseHeader(const std::string &dir_path)
        : m_dir_path(dir_path) {}

void http::ResponseHeader::add_header(const std::string &key, const std::string &value) {
    m_misc_headers.insert(std::make_pair(key, value));
}

std::string http::ResponseHeader::serialize(const RequestHeader &req_header) {
    // 1. open file (return empty string if not works; eventually replace with exception)
    // 2. create response line
    // 3. iterate over headers and add them
    // 4. add content of opened file
    // 5. return file
    std::string response;
    std::string file_path = std::string(m_dir_path + "/" + req_header.get_uri());    // TODO: check if "/" works

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
        if (m_misc_headers.empty() == false) {
            for (auto const &[key, val] : m_misc_headers) {
                response.append(key);
                response.append(": ");
                response.append(val);
                response.append(constants::LINE_ENDING);
            }
        }

        response.append("Content-Length: ");
        response.append(std::to_string(content.size()));
        response.append(constants::LINE_ENDING);    // two CRLF sequences needed to indicate start of content
        response.append(constants::LINE_ENDING);
        response.append(content);
    } else if (std::filesystem::exists(file_path) == false) {
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

