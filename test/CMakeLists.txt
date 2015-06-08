set (GTEST_DIR "../../../thirdParty/gtest-1.7.0")
set (CODE_DIR $ENV{HOME}/code)
set (MYJSON_DIR "${CODE_DIR}/cpp/MyJson")
set (JSONCPP_DIR ${CODE_DIR}/thirdParty/jsoncpp/dist)
set (LOGGER_DIR ${CODE_DIR}/cpp/Logger/src)
set (DATETIME_DIR ${CODE_DIR}/cpp/DateTime)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        option( gtest_force_shared_crt
                "Use shared (DLL) run-time lib even when Google Test is built as static lib."
                ON)

endif()

include_directories ( 	${GTEST_DIR}/include
                        ${MYJSON_DIR}/src
                        ${LOGGER_DIR}
                        ${JSONCPP_DIR}
                        ${DATETIME_DIR}
                        ${CODE_DIR}/cpp/include)

link_directories (${GTEST_DIR}/build-mac   ${MYJSON_DIR}/build)

add_executable (HttpClientTest httpclient_test.cpp  ${LOGGER_DIR}/logger.cpp
                         ${JSONCPP_DIR}/jsoncpp.cpp  ${DATETIME_DIR}/datetime.cpp)

target_link_libraries (HttpClientTest 	HttpClient gtest MyJson)

add_test(HttpClientTest HttpClientTest)
