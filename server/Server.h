#ifndef SIMPLE_SERVER_H
#define SIMPLE_SERVER_H

#include "boost/asio.hpp"
#include <boost/system/error_code.hpp>
#include <fstream>
#include <string>
#include <iostream>


class server
{
private:
    void accept_file(boost::asio::ip::tcp::socket& socket, std::string& path);
    void send_file(boost::asio::ip::tcp::socket& socket, const std::string& file_path);
public:
    int run();
};



#endif 