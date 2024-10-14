#ifndef CLIENT_H
#define CLIENT_H

#include "boost/asio.hpp"
#include <boost/system/error_code.hpp>
#include <fstream>
#include <iostream>
#include <string>


class client
{
private:
    void send_file(boost::asio::ip::tcp::socket& socket, std::string& path);

    void receive_file(boost::asio::ip::tcp::socket &socket, const std::string &path);

public:
    void run();
};


#endif