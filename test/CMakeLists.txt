add_executable(
        test_parser_all
        test_requestparser.cpp
        ../src/http_request.cpp
)

include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(test_parser_all ${GTEST_LIBRARIES} pthread)
