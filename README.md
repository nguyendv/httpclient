# README #

Small HttpClient library.

### Usage ###

* Add http-client.h and http-client.cpp to your source.
* Add libboost-system dependency (this lib depends on asio, asio is just a hpp file, but it depends on boost system)
```
set(Boost_INCLUDE_DIR ../../thirdParty/boost_1_57_0)
set(Boost_LIBRARY_DIR ../../thirdParty/boost_1_57_0/stage/lib)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system REQUIRED)

target_link_libraries (YourApp ${Boost_LIBRARIES})
```