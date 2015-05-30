cmake_minimum_required(VERSION 3.2)

project("HttpClient")

set (CODE_DIR "/Users/vietdung2910/code")
set (ASIO_INCLUDE_DIR ${CODE_DIR}/thirdParty/asio/asio/include )


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -fno-exceptions -fno-rtti")

add_library(HttpClient STATIC	httpclient.cpp    )

add_definitions(-DASIO_STANDALONE -DASIO_NO_EXCEPTIONS  -DASIO_NO_TYPEID  )

include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                     ${ASIO_INCLUDE_DIR}
                     )
#link_directories
target_link_libraries (HttpClient )

enable_testing()

add_subdirectory(test)
