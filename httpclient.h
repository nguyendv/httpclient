#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H


#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>
#include <string>

using boost::asio::ip::tcp;
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

        /**
         * @brief set_x_api_key set X-Api-Key header for the request
         */
        void    set_x_api_key(const string& x_api_key){_x_api_key = x_api_key;}

        /**
         * @brief set_access_token set OAuth2 access token for the request
         */
        void    set_access_token(const string& access_token){_access_token = access_token;}

        /**
         * @brief get set GET request
         */
        string 	get (const string& path, data_type type=HTML);

        /**
         * @brief post post data to server
         * @param path
         * @param post_data
         * @param type HTML: post_data is form data. type JSON: post_data is json string
         * @return
         */
        string  	post(const string& path, const string& post_data="", data_type type=HTML);
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
