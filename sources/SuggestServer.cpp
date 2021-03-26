// Copyright 2020 Your Name <your_email>

#include <SuggestServer.hpp>

void SuggestServer::StartServer() {
  boost::asio::io_service ioService;
  boost::asio::ip::tcp::endpoint endPoint(address_, port_);
  boost::asio::ip::tcp::acceptor acceptor(ioService, endPoint);
}
