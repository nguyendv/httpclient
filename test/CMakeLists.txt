set (GTEST_DIR "../../../thirdParty/gtest-1.7.0")
set (CODE_DIR "/Users/vietdung2910/code")
set (MYJSON_DIR "${CODE_DIR}/cpp/MyJson")
set (RAPID_JSON_DIR "${CODE_DIR}/thirdParty/rapidjson")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        option( gtest_force_shared_crt
                "Use shared (DLL) run-time lib even when Google Test is built as static lib."
                ON)

endif()

include_directories ( 	${GTEST_DIR}/include
                        ${MYJSON_DIR}/src
                        ${RAPID_JSON_DIR}/include)

link_directories (${GTEST_DIR}/build-mac   ${MYJSON_DIR}/build)

add_executable (HttpClientTest httpclient_test.cpp)

target_link_libraries (HttpClientTest 	HttpClient gtest MyJson)

add_test(HttpClientTest HttpClientTest)
