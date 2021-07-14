#include <stdlib.h>
#include <iostream>
#include <string>

#include "Server.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    try 
    {
        if (argc != 7)
        {
            std::cout << "Wrong arguments, it must be <server_address> <port> <db_server> <MySQL_User> <MySQL_Password> <db_schema>" << std::endl;
            return 1;
        }
        HTTPServer server(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

        //server.run();
    }
    catch (sql::SQLException& e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }

    return 0;
}