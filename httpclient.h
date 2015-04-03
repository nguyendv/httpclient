#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H


#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>
#include <string>
#include <json.hpp>

using boost::asio::ip::tcp;
using json = nlohmann::json;
using namespace std;



namespace http_client {

    enum data_type {
      JSON,
      HTML
    };

    class HttpClient{
    public:
        HttpClient (const string& host, const string& port="");
        ~HttpClient();
        void    set_x_api_key(const string& x_api_key){_x_api_key = x_api_key;}
        void    set_access_token(const string& access_token){_access_token = access_token;}
        string 	get (const string& path, data_type type=HTML);
        json	getJSON (const string& path);

        /**
         * @brief postJSON post json data to server
         * @param host
         * @param port
         * @param path
         * @param post_data string representation of the post data
         * @return json return from server
         */
        json  	postJSON(const string& path, const string& post_data);
    private:
        boost::asio::io_service _io_service;
        tcp::resolver::iterator _endpoint_iter;
        tcp::socket*            _psocket;
        tcp::resolver::query*   _pquery;
        string					_host;

        string					_x_api_key;
        string					_access_token;
    };
} //namespace http_client

#endif // HTTP_CLIENT_H
