cmake_minimum_required(VERSION 3.2)

project("HttpClient")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(Boost_INCLUDE_DIR ../../thirdParty/boost_1_57_0)
set(Boost_LIBRARY_DIR ../../thirdParty/boost_1_57_0/stage/lib)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system REQUIRED)

#add_executable(HttpClient httpclient.cpp main.cpp)
add_library(HttpClient STATIC	httpclient.cpp)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                     ${Boost_INCLUDE_DIR}
                     ../../thirdParty/json/src
                    )
#link_directories
target_link_libraries (HttpClient ${Boost_LIBRARIES})

enable_testing()

add_subdirectory(test)
