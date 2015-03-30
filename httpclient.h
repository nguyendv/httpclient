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
        string 	get (const string& host, const string& port, const string& path, data_type type=HTML);
        json	getJSON (const string& host, const string& port, const string& path);

        /**
         * @brief postJSON post json data to server
         * @param host
         * @param port
         * @param path
         * @param post_data string representation of the post data
         * @return json return from server
         */
        json  	postJSON(const string& host, const string& port,
                         const string& path, const string& api_key,
                         const string& post_data);

    };
} //namespace http_client

#endif // HTTP_CLIENT_H
