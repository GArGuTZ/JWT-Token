#include "Server.h"

HTTPServer::HTTPServer(const std::string& _address, const std::string& _port,
    const std::string& _db_server, const std::string& _db_user, const std::string& _db_password, const std::string& _schema)
    : io_context_(1),
    acceptor_(io_context_)
{
    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(_address, _port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();


    driver_ = get_driver_instance(); 
    std::unique_ptr<sql::Connection>mysqlConnection(driver_->connect(_db_server, _db_user, _db_password));
    mysqlConnection->setSchema(_schema);
    std::unique_ptr<sql::Statement> statement(mysqlConnection->createStatement());
    std::unique_ptr<sql::ResultSet> resultSet(statement->executeQuery("SELECT * FROM users"));

    std::cout << "SELECT * FROM users" << std::endl;
    while (resultSet->next()) 
    {
        std::cout 
            << resultSet->getString("user_id") << ' ' 
            << resultSet->getString("username") << ' '
            << resultSet->getString("password") << std::endl;
    }
}

void HTTPServer::run()
{
    io_context_.run();
}
