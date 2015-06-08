cmake_minimum_required(VERSION 3.2)

project("HttpClient")

set (CODE_DIR $ENV{HOME}/code)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -fno-exceptions -fno-rtti")

add_library(HttpClient STATIC	httpclient.cpp    )

#add_definitions()

include_directories (	${CMAKE_CURRENT_SOURCE_DIR}
                        ${CODE_DIR}/cpp/include
                     )
#link_directories
target_link_libraries (HttpClient )

enable_testing()

add_subdirectory(test)
