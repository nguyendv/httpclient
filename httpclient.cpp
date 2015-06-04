#include "httpclient.h"
#include "errors.h"
#include <sstream>

namespace http_client {


HttpClient::HttpClient(const string &host, const string &port)
{
    _error = NO_ERROR;
    _server_socket = MY_INVALID_SOCKET;

    myaddrinfo hints, *server_info;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    string service;
    if (port.empty())
        service = "http";
    else
        service = port;
    int status = mygetaddrinfo (host.c_str(), service.c_str(), &hints, &server_info);
    if (status != 0){
        _error = ADDR_INFO_ERROR;
        return;
    }

    myaddrinfo *p;
    for (p=server_info; p!=NULL; p=p->ai_next){
        _server_socket = mycreatesocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_server_socket == MY_INVALID_SOCKET){
            _error = INVALID_SOCKET_ERROR;
            continue;
        }

        if (myconnect(_server_socket, p->ai_addr, p->ai_addrlen) != 0){
            _error = CONNECT_SOCKET_ERROR;
            continue;
        }

        break;
    }

    if (p==NULL){
        return;
    }

    freeaddrinfo(server_info);

    _host = host;
}

HttpClient::~HttpClient()
{
}

string HttpClient::get(const string &path, data_type type)
{
    assert (GetErrorCode() == NO_ERROR);
    std::ostringstream request_stream;
    request_stream << "GET " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << _host << "\r\n";
    if (type == data_type::JSON)
        request_stream << "Accept: application/json \r\n";
    else
        request_stream << "Accept: */* \r\n";
    if (_x_api_key != "")
        request_stream << "X-Api-Key: " << _x_api_key << "\r\n";
    if (_access_token != "")
        request_stream << "Authorization: Bearer " << _access_token << "\r\n";
    request_stream << "Connection: close \r\n\r\n";	// close socket connection after get all response

    return getResponse(request_stream.str());
}

string HttpClient::post(const string& path, const string& post_data, data_type type)
{
    assert (GetErrorCode() == NO_ERROR);

    std::ostringstream request_stream;
    request_stream << "POST " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << _host << "\r\n";
    if (type == JSON)
        request_stream << "Accept: application/json \r\n";
    else
        request_stream << "Accept: */* \r\n";
    if (_x_api_key != "")
        request_stream << "X-Api-Key: " << _x_api_key << "\r\n";
    if (_access_token != "")
        request_stream << "Authorization: Bearer " << _access_token << "\r\n";
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

    if (mysendall(_server_socket, request.c_str(), request.size()) == -1){
        return "error: \"send request error\"";
    }

    std::ostringstream response_stream;
    char buf[1024];
    while(1){
        memset(buf, 0, sizeof buf);
        int bytes_received = myrecv(_server_socket, buf, sizeof buf, 0);
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
