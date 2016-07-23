#include "httpclient.h"
#include "exception.h"
#include <assert.h>
#include <sstream>
#include <cstring>

#include <iostream>


namespace httpclient {
const int NO_ERROR = 0;
const int ADDR_INFO_ERROR = 1;
const int INVALID_SOCKET_ERROR = 2;
const int CONNECT_SOCKET_ERROR = 3;

// TODO: merge https client into this library


HttpClient::HttpClient(const string& hostStr) 
  : error_(0), 
    server_socket_(MY_INVALID_SOCKET),
    host_(hostStr)
{
}

HttpClient::~HttpClient()
{
}

void HttpClient::connectToHost()
{
    myaddrinfo hints, *server_info;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Get server information
    int status = mygetaddrinfo (host_.hostName().c_str(), host_.service().c_str(), &hints, &server_info);

    if (status != 0){
      freeaddrinfo(server_info);
      throw httpclient::Exception("Can't get host address info");
    }

    // Try to connect to server using server info
    myaddrinfo *p;

    // Loop over the linked list server_info, return the first connected one
    // Because a server may provide multiple records/ip versions, looping is necessary
    for (p=server_info; p!=NULL; p=p->ai_next){
        server_socket_ = mycreatesocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_socket_ == MY_INVALID_SOCKET){
          continue;
        }

        if (myconnect(server_socket_, p->ai_addr, p->ai_addrlen) != 0){
          continue;
        }

        break;
    }

    if(p == NULL){
      freeaddrinfo(server_info);
      throw httpclient::Exception("Can't connect to server");
    }

    freeaddrinfo(server_info);
}

string HttpClient::get(const string &path, data_type type)
{
    connectToHost();
    std::ostringstream request_stream;
    request_stream << "GET " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host_.hostName() << "\r\n";
    if (type == data_type::JSON)
        request_stream << "Accept: application/json \r\n";
    else
        request_stream << "Accept: */* \r\n";
    if (x_api_key_ != "")
        request_stream << "X-Api-Key: " << x_api_key_ << "\r\n";
    if (access_token_ != "")
        request_stream << "Authorization: Bearer " << access_token_ << "\r\n";
    request_stream << "Connection: close \r\n\r\n";	// close socket connection after get all response

    return getResponse(request_stream.str());
}

string HttpClient::post(const string& path, const string& post_data, data_type type)
{
    connectToHost();

    std::ostringstream request_stream;
    request_stream << "POST " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host_.hostName() << "\r\n";
    if (type == JSON)
        request_stream << "Accept: application/json \r\n";
    else
        request_stream << "Accept: */* \r\n";
    if (x_api_key_ != "")
        request_stream << "X-Api-Key: " << x_api_key_ << "\r\n";
    if (access_token_ != "")
        request_stream << "Authorization: Bearer " << access_token_ << "\r\n";
    if (type == JSON)
        request_stream << "Content-type: application/json \r\n";
    if (post_data != ""){
        int content_length = post_data.length();
        request_stream << "Content-length: " << content_length << "\r\n";
    }
    request_stream << "Connection: close \r\n\r\n";

    if(post_data != "")
        request_stream << post_data;

    return getResponse(request_stream.str());
}



string HttpClient::getResponse(const string &request){

    if (mysendall(server_socket_, request.c_str(), request.size()) == -1){
        return "error: \"send request error\"";
    }

    std::ostringstream response_stream;
    char buf[1024];
    while(1){
        memset(buf, 0, sizeof buf);
        int bytes_received = myrecv(server_socket_, buf, sizeof buf, 0);
        if (bytes_received > 0){
            for(int i = 0; i<bytes_received; ++i)
                response_stream << buf[i];
            continue;
        }
        if (bytes_received == 0){
            break;
        }
        if (bytes_received < 0){
            return "error: \"receive response error \"";
        }
    }

    string raw_response = response_stream.str();
    size_t header_end_index = raw_response.find("\r\n\r\n");
    if (header_end_index == string::npos){
        return "error: \"invalid response\"";
    }
    return raw_response.substr(header_end_index+4);
}

} // namespace http_client
