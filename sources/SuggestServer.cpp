// Copyright 2020 Your Name <your_email>

#include <SuggestServer.hpp>


void RequestHandler(boost::asio::ip::tcp::socket&& socket,
                    boost::string_view directory) {

}
void SuggestServer::StartServer() {
  boost::asio::io_context ioContext;
  boost::asio::ip::tcp::endpoint endPoint(address_, port_);
  boost::asio::ip::tcp::acceptor acceptor(ioContext, endPoint);

  for(;;){
    boost::asio::ip::tcp::socket socket1{ioContext};
    acceptor.accept(socket1);
    std::thread([&](){
      RequestHandler(std::move(socket1), URL_);
    }).detach();
  }
}

