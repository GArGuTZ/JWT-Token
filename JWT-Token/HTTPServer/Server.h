#pragma once

#include <stdlib.h>
#include <iostream>
#include <string>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <boost/asio.hpp>
#include <string>

class HTTPServer
{
public:
    HTTPServer(const HTTPServer&) = delete;
    HTTPServer& operator=(const HTTPServer&) = delete;

    explicit HTTPServer(const std::string& _address, const std::string& _port, 
        const std::string& _db_server, const std::string& _db_user, const std::string& _db_password, const std::string& _schema);

    void run();
private:
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    sql::Driver* driver_;
};
