set (GTEST_DIR "../../../thirdParty/gtest-1.7.0")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        option( gtest_force_shared_crt
                "Use shared (DLL) run-time lib even when Google Test is built as static lib."
                ON)

endif()

include_directories ( ${GTEST_DIR}/include )

link_directories (${GTEST_DIR}/build-mac )

add_executable (HttpClientTest httpclient_test.cpp)

add_executable (HttpsClientTest httpclient_test.cpp)
set_property(TARGET HttpsClientTest PROPERTY COMPILE_DEFINITIONS HTTPS)

target_link_libraries (HttpClientTest 	HttpClient gtest)
target_link_libraries (HttpsClientTest 	HttpsClient gtest)

add_test(HttpClientTest HttpClientTest)
add_test(HttpsClientTest HttpsClientTest)
