#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "httpclient.h"

using namespace httpclient;

TEST_CASE( "Http get", "[get]"){
  HttpClient httpClient("127.0.0.1", "3000");
  httpClient.set_x_api_key("blobla");
  string res = httpClient.get("/favbook");
  REQUIRE (res.compare("Harry Porter") == 0);
}

