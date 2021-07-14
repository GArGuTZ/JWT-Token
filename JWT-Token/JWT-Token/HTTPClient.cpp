#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree; using boost::property_tree::read_json; using boost::property_tree::write_json;
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    std::setlocale(LC_ALL, "Russian");
    try
    {
        if (argc != 2)
        {
            std::cout << "Wrong arguments, it must be only <server-address>\n";
            return 1;
        }


        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "12345");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);


        std::string username;
        std::cout << "Type your UserName: " << std::endl;
        std::getline(std::cin, username);
        std::string user_password;
        std::cout << "Type your Password: " << std::endl;
        std::getline(std::cin, user_password);
        //std::cout << "Entered strings is " << username << ' ' << user_password;
        ptree json_root;
        json_root.put("name", username);
        json_root.put("password", user_password);
        std::ostringstream buffer;
        write_json(buffer, json_root, false);
        std::string user_passw_json = buffer.str();
        //std::cout << user_passw_json << std::endl;

        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "POST /title/ HTTP/1.0 \r\n";
        request_stream << "Host:" << argv[1] << "\r\n";
        request_stream << "User-Agent: C/1.0\r\n";
        request_stream << "Content-Type: application/json; charset=utf-8 \r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Content-Length: " << user_passw_json.length() << "\r\n";
        request_stream << "Connection: close\r\n\r\n";
        request_stream << user_passw_json;

        boost::asio::write(socket, request);


        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        // Check that response is OK.-----> second cin with message
        std::istream response_stream(&response);
        /***************/
        //std::string  full_line;
        //std::getline(response_stream, full_line);
        //std::cout << full_line << std::endl;
        /***************/

        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
            return 1;
        }
        if (status_code != 200)
        {
            std::cout << "Response returned with status code " << status_code << "\n";
            std::cout << "Status message length " << status_message.length() << "\n";
            std::cout << status_message << "\n";
            return 1;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
        {
            std::cout << header << "\n";
        }
        std::cout << "\n";

        // Write whatever content we already have to output.
        if (response.size() > 0)
        {
            std::cout << &response;
        }

        // Read until EOF, writing data to output as we go.
        boost::system::error_code error;
        while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
        {
            std::cout << &response;
        }
        if (error != boost::asio::error::eof)
        {
            throw boost::system::system_error(error);
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}