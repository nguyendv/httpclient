#include "httpclient/response.h"

using namespace http;

Response::Response(const string& body)
  : body_(body)
{

}

string Response::body()
{
  return body_;
}
