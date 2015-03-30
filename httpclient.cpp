#include "httpclient.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>


using boost::asio::ip::tcp;

namespace http_client {

string HttpClient::get(const string &host,const string &port, const string &path, data_type type)
{
    try
        {
            boost::asio::io_service io_service;

            // Get a list of endpoints
            tcp::resolver resolver(io_service);

            tcp::resolver::query query(host, "http");
            if (port != "")
                query = tcp::resolver::query(host, port);


            tcp::resolver::iterator endpoint_iter = resolver.resolve(query);

            // Try each endpoint util we successfull establish a connection
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iter);

            // Form the request. We specify the "Connection: close" header so that the s
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "GET " << path << " HTTP/1.1\r\n";
            request_stream << "Host: " << host << "\r\n";
            if (type == data_type::JSON)
                request_stream << "Accept: application/json \r\n";
            else
                request_stream << "Accept: */* \r\n";
            request_stream << "Connection: close \r\n\r\n";

            // Send the request
            boost::asio::write(socket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, "\r\n");
            // Check that response is OK
            std::istream response_stream(&response);
            std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);

            if (!response_stream || http_version.substr(0,5) != "HTTP/")
            {
                return "error: Invalid response";
            }

            if (status_code != 200)
            {
                std::cout << "status_code: " << status_code << "\n";
                return "error: Status Code not 2000";
            }

            // Read the response headers, which are terminated by a black line.
            boost::asio::read_until(socket, response, "\r\n\r\n");
            // Process the response headers
            std::string header;
            while (std::getline(response_stream, header) && header != "\r"){
            //do something with this header
            }

            // Read until EOF
            boost::system::error_code ec;
            std::stringstream ret;
            while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), ec))
                ret << &response;
            if ( ec != boost::asio::error::eof){
                throw boost::system::system_error(ec);
            }

            return ret.str();
        }
        catch (std::exception& e)
        {
            std::cout << "Exception: " << e.what() << "\n\n";
            return std::string("\"error\":")  + std::string(e.what());
        }
}

    json HttpClient::getJSON (const string& host, const string& port, const string& path){
        try{
            return json::parse(get(host,port,path,data_type::JSON));
        }catch (std::exception & e){
            std::cout << "Exception: " << e.what() << "\n\n";
            return json::parse(std::string("\"error\":")  + std::string(e.what()));
        }
    }


    json HttpClient::postJSON(const string& host, const string& port,
                              const string& path, const string& api_key,
                              const string& post_data)
    {
        try
        {
            boost::asio::io_service io_service;

            // Get a list of endpoints
            tcp::resolver resolver(io_service);

            tcp::resolver::query query(host, "http");
            if (port != "")
                query = tcp::resolver::query(host, port);


            tcp::resolver::iterator endpoint_iter = resolver.resolve(query);

            // Try each endpoint util we successfull establish a connection
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iter);

            // Form the request. We specify the "Connection: close" header so that the s
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "POST " << path << " HTTP/1.1\r\n";
            request_stream << "Host: " << host << "\r\n";
            request_stream << "Accept: application/json \r\n";
            request_stream << "X-Api-Key: " << api_key << "\r\n";
            request_stream << "Content-type: application/json \r\n";
            int content_length = post_data.length();
            request_stream << "Content-length: " << content_length << "\r\n";
            request_stream << "Connection: close \r\n\r\n";

            request_stream << post_data;

            // Send the request
            boost::asio::write(socket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, "\r\n");
            // Check that response is OK
            std::istream response_stream(&response);
            std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);

            if (!response_stream || http_version.substr(0,5) != "HTTP/")
            {
                return json::parse("{\"error\": \"Invalid response\"}");
            }

            if (status_code != 200)
            {
                std::cout << "status_code: " << status_code << "\n";
                return json::parse("{\"error\": \"Status Code not 2000\"}");
            }

            // Read the response headers, which are terminated by a black line.
            boost::asio::read_until(socket, response, "\r\n\r\n");
            // Process the response headers
            std::string header;
            while (std::getline(response_stream, header) && header != "\r"){
            //do something with this header
            }

               // Read until EOF
               boost::system::error_code ec;
               std::stringstream ret;
               while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), ec))
                   ret << &response;
               if ( ec != boost::asio::error::eof){
                   throw boost::system::system_error(ec);
               }

               return json::parse(ret.str());
           }
           catch (std::exception& e)
           {
               std::cout << "Exception: " << e.what() << "\n\n";
               return json::parse(std::string("\"error\":")  + std::string(e.what()));
           }
   }

} // namespace http_client
