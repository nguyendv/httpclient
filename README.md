# README #

Small HttpClient library.

From the v1.0.0, we use raw socket instead of asio socket. The code using asio can be found in the asio branch.

### Usage (Using asio) ###

* Add http-client.h and http-client.cpp to your source.
* Add definition: ASIO_STANDALONE
* If you don't enable exception and RTTI, add these definitions: ASIO_NO_EXCEPTIONS and ASIO_NO_TYPEID.