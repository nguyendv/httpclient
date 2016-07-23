#include "httpclient/host.h"

using namespace httpclient;

Host::Host(const string& host) : service_("http")
{
  parseInfo(host); 
}

void Host::parseInfo(const string& host)
{
  // Find the position of ':'
  size_t colonPos = host.find(":");

  // Determine the value of hostName_ and service_ depend on the position of ':'
  if (colonPos != string::npos){
    service_ = host.substr(colonPos + 1);
    hostName_ = host.substr(0, colonPos );
  }
  else{
    service_ = "http";
    hostName_ = host;
  }
}
