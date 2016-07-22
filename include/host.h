#pragma once
/*
 * Host represents a httphost, which contains a hostname and an optional port number
 * Examples of hosts are: "123.184.1.1", "13.33.32.1:5000", "example.com", "example.com:5000"
 */
#include <string>

using namespace std;

namespace httpclient
{
  class Host
  {
  public:
    Host(const string& host);

    string service() const{
      return service_;
    }

    string hostName() const{
      return hostName_;
    }

  private:
    // Parse the 'host' passed to constructor; in order to get hostname and service from that info
    void parseInfo(const string& host);

    string hostName_;
    
    // A service represents either service name, such as 'http', 'echo'; or a port number passed as string
    // Since we are writing http client, I assume the service is just 'http' or portnumber
    // So service_ is 'http' by default
    string service_; 
  };
} // namesapce httpclient
