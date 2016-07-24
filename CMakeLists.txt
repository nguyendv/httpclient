cmake_minimum_required(VERSION 3.2)

project("HttpClient")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-c++11-extensions")

add_library(httpclient SHARED 
  src/client.cpp    
  src/host.cpp
  src/response.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/net/src/streamsocket_unix.cpp
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/net/include
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/catch
)

enable_testing()
add_subdirectory(test)
