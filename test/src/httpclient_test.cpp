#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "httpclient/client.h"

using namespace http;

#include <iostream>

TEST_CASE ("Http Host", "[]")
{
  Host host1("123.4.22.55:3042");
  REQUIRE ( host1.hostName().compare("123.4.22.55") == 0);
  REQUIRE ( host1.service().compare("3042") == 0);

  Host host2("www.example.com");
  REQUIRE ( host2.hostName().compare("www.example.com") == 0);
  REQUIRE ( host2.service().compare("http") == 0);
}

TEST_CASE( "Http get", "[get]")
{
  Client httpClient("127.0.0.1:3000");
  httpClient.set_x_api_key("blobla");
  Response res = httpClient.get("/favbook");
  REQUIRE (res.body().compare("Harry Porter") == 0);
}

