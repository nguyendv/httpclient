#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <iostream>
#include <string>

#include "httpclient/host.h"
#include "httpclient/response.h"

#include "net/streamsocket.h"

using namespace std;


namespace http {

    class Client{
    public:
        Client (const string& hostStr); 
        ~Client();

        int GetErrorCode()
        {
            return error_;
        }

        /**
         * @brief set_x_api_key set X-Api-Key header for the request
         */
        void    set_x_api_key(const string& x_api_key){x_api_key_ = x_api_key;}

        /**
         * @brief set_access_token set OAuth2 access token for the request
         */
        void    set_access_token(const string& access_token){access_token_ = access_token;}

        /**
         * @brief get set GET request
         */
        Response get (const string& path );

        /**
         * @brief post post data to server
         * @param path
         * @param post_data
         * @return
         */
        Response post(const string& path, const string& post_data="");
    private:
        
        void connectToHost();
        Response getResponse(const string& request);

        net::StreamSocket streamSock_;
        int				error_;

        Host host_;
        string			x_api_key_;
        string			access_token_;
    };
} //namespace http

#endif // HTTP_CLIENT_H
