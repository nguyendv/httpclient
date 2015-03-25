#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

namespace http_client {

    enum data_type {
      JSON,
        HTML
    };

    class HttpClient{
    public:
        string 					get (const string& host, const string& port, const string& path, data_type type=HTML);
        json					getJSON (const string& host, const string& port, const string& path);
    };
} //namespace http_client

#endif // HTTP_CLIENT_H
