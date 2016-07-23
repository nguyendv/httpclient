#include "httpclient.h"
#include "exception.h"
#include <assert.h>
#include <sstream>
#include <cstring>

#include "net/streamsocket.h"
#include "net/exception.h"

#include <iostream>


namespace httpclient {
const int NO_ERROR = 0;
const int ADDR_INFO_ERROR = 1;
const int INVALID_SOCKET_ERROR = 2;
const int CONNECT_SOCKET_ERROR = 3;

// TODO: merge https client into this library


HttpClient::HttpClient(const string& hostStr) 
  : error_(0), 
    host_(hostStr)
{
}

HttpClient::~HttpClient()
{
}

void HttpClient::connectToHost()
{
  try
  {
    streamSock_.connectToHost(host_.hostName(), host_.service());
  }
  catch (const net::Exception& ex)
  {
    cout << ex.what();
    throw httpclient::Exception("Can't connect to server");
  }
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

  try
  {
    streamSock_.sendMsg(request);
  }
  catch (const net::Exception& ex)
  {
    throw httpclient::Exception("Couldn't send request to server");
  }

  try
  {
    string raw_response = streamSock_.receiveMsg();
    size_t header_end_index = raw_response.find("\r\n\r\n");
    if (header_end_index == string::npos){
      throw httpclient::Exception("Invalid response from server");
    }
    return raw_response.substr(header_end_index+4);
  }
  catch (const net::Exception& ex)
  {
    throw httpclient::Exception("Couldn't receive response from server");
  } 

}

} // namespace http_client
