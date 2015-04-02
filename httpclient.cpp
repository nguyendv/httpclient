#include "httpclient.h"





namespace http_client {


HttpClient::HttpClient(const string &host, const string &port)
{
    try{
        // Get a list of endpoints
        tcp::resolver resolver(_io_service);
        if (port == "")
            _pquery = new tcp::resolver::query(host, "http");
        else
            _pquery = new tcp::resolver::query(host, port);
        _endpoint_iter = resolver.resolve(*_pquery);

        // Try each endpoint util we successfull establish a connection
        _psocket = new tcp::socket(_io_service);
        boost::asio::connect(*_psocket, _endpoint_iter);
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n\n";
    }

    _host = host;
}

HttpClient::~HttpClient()
{
    if (_pquery != NULL)
        delete _pquery;
    if (_psocket != NULL)
        delete _psocket;
}

string HttpClient::get(const string &path, data_type type)
{
    try
        {
            // Form the request. We specify the "Connection: close" header so that the s
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "GET " << path << " HTTP/1.1\r\n";
            request_stream << "Host: " << _host << "\r\n";
            if (type == data_type::JSON)
                request_stream << "Accept: application/json \r\n";
            else
                request_stream << "Accept: */* \r\n";
            request_stream << "Connection: close \r\n\r\n";

            // Send the request
            boost::asio::write(*_psocket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::asio::streambuf response;
            boost::asio::read_until(*_psocket, response, "\r\n");
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
            boost::asio::read_until(*_psocket, response, "\r\n\r\n");
            // Process the response headers
            std::string header;
            while (std::getline(response_stream, header) && header != "\r"){
            //do something with this header
            }

            // Read until EOF
            boost::system::error_code ec;
            std::stringstream ret;
            while (boost::asio::read(*_psocket, response, boost::asio::transfer_at_least(1), ec))
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

    json HttpClient::getJSON (const string& path){
        try{
            return json::parse(get(path,data_type::JSON));
        }catch (std::exception & e){
            std::cout << "Exception: " << e.what() << "\n\n";
            return json::parse(std::string("\"error\":")  + std::string(e.what()));
        }
    }


    json HttpClient::postJSON(const string& path, const string& post_data,
                              const string& api_key)
    {
        try
        {
            // Form the request. We specify the "Connection: close" header so that the s
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "POST " << path << " HTTP/1.1\r\n";
            request_stream << "Host: " << _host << "\r\n";
            request_stream << "Accept: application/json \r\n";
            if (api_key != "")
                request_stream << "X-Api-Key: " << api_key << "\r\n";
            request_stream << "Content-type: application/json \r\n";
            int content_length = post_data.length();
            request_stream << "Content-length: " << content_length << "\r\n";
            request_stream << "Connection: close \r\n\r\n";

            request_stream << post_data;

            // Send the request
            boost::asio::write(*_psocket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::asio::streambuf response;
            boost::asio::read_until(*_psocket, response, "\r\n");
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
            boost::asio::read_until(*_psocket, response, "\r\n\r\n");
            // Process the response headers
            std::string header;
            while (std::getline(response_stream, header) && header != "\r"){
            //do something with this header
            }

               // Read until EOF
               boost::system::error_code ec;
               std::stringstream ret;
               while (boost::asio::read(*_psocket, response, boost::asio::transfer_at_least(1), ec))
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
