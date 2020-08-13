#include "gtest/gtest.h"
#include "../inc/http_request.h"

TEST(RequestParserTest, ChromeRequest) {
    std::string req_str = "GET / HTTP/1.1\r\n"
                          "Host: localhost:8080\r\n"
                          "Connection: keep-alive\r\n"
                          "Upgrade-Insecure-Requests: 1\r\n"
                          "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36\r\n"
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
                          "Sec-Fetch-Site: none\r\n"
                          "Sec-Fetch-Mode: navigate\r\n"
                          "Sec-Fetch-User: ?1\r\n"
                          "Sec-Fetch-Dest: document\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Accept-Language: en,de-AT;q=0.9,de;q=0.8,en-US;q=0.7,de-DE;q=0.6\r\n\r\n";

    http::RequestHeader req_header(req_str);
    EXPECT_EQ(req_header.get_uri(), "index.html");
    EXPECT_EQ(req_header.get_header("Host"), "localhost:8080");
    EXPECT_EQ(req_header.get_header("Connection"), "keep-alive");
    EXPECT_EQ(req_header.get_header("Upgrade-Insecure-Requests"), "1");
    EXPECT_EQ(req_header.get_header("Sec-Fetch-Site"), "none");
    EXPECT_EQ(req_header.get_header("Sec-Fetch-Mode"), "navigate");
    EXPECT_EQ(req_header.get_header("Sec-Fetch-User"), "?1");
    EXPECT_EQ(req_header.get_header("Sec-Fetch-Dest"), "document");
    EXPECT_EQ(req_header.get_header("Accept-Encoding"), "gzip, deflate, br");
    EXPECT_EQ(req_header.get_header("Accept-Language"), "en,de-AT;q=0.9,de;q=0.8,en-US;q=0.7,de-DE;q=0.6");
    EXPECT_EQ(req_header.get_header("User-Agent"),
              "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36");
    EXPECT_EQ(req_header.get_header("Accept"),
              "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
}

TEST(RequestParserTest, FirefoxRequest) {
    std::string req_str = "GET / HTTP/1.1\r\n"
                          "Host: localhost:8080\r\n"
                          "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0\r\n"
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                          "Accept-Language: en-US,en;q=0.5\r\n"
                          "Accept-Encoding: gzip, deflate\r\n"
                          "Connection: keep-alive\r\n"
                          "Upgrade-Insecure-Requests: 1\r\n\r\n";

    http::RequestHeader req_header(req_str);
    EXPECT_EQ(req_header.get_uri(), "index.html");
    EXPECT_EQ(req_header.get_header("Host"), "localhost:8080");
    EXPECT_EQ(req_header.get_header("Accept-Encoding"), "gzip, deflate");
    EXPECT_EQ(req_header.get_header("Accept-Language"), "en-US,en;q=0.5");
    EXPECT_EQ(req_header.get_header("Connection"), "keep-alive");
    EXPECT_EQ(req_header.get_header("Upgrade-Insecure-Requests"), "1");
    EXPECT_EQ(req_header.get_header("User-Agent"),
              "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0");
    EXPECT_EQ(req_header.get_header("Accept"),
              "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
}

TEST(RequestParserTest, WgetRequest) {
    std::string req_str = "GET / HTTP/1.1\r\n"
                          "User-Agent: Wget/1.20.3 (linux-gnu)\r\n"
                          "Accept: */*\r\n"
                          "Accept-Encoding: identity\r\n"
                          "Host: localhost:8080\r\n"
                          "Connection: Keep-Alive\r\n\r\n";

    http::RequestHeader req_header(req_str);
    EXPECT_EQ(req_header.get_uri(), "index.html");
    EXPECT_EQ(req_header.get_header("User-Agent"), "Wget/1.20.3 (linux-gnu)");
    EXPECT_EQ(req_header.get_header("Accept"), "*/*");
    EXPECT_EQ(req_header.get_header("Accept-Encoding"), "identity");
    EXPECT_EQ(req_header.get_header("Host"), "localhost:8080");
    EXPECT_EQ(req_header.get_header("Connection"), "Keep-Alive");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
