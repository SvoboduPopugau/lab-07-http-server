// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <beast/http/message.hpp>
#include <beast/http/read.hpp>
#include <beast/http/string_body.hpp>
#include <thread>
#include <iostream>
#include <beast.hpp>
#include <nlohmann/json.hpp>

class SuggestServer{
 public:
  explicit SuggestServer(boost::string_view& URL,
                         boost::asio::ip::address& address,
                         unsigned port):  URL_(URL),
                                          address_(address),
                                          port_(port){};
  void StartServer();

 private:
  boost::string_view URL_;
  boost::asio::ip::address address_;
  unsigned port_;
  nlohmann::json j_data_;
  std::mutex mutex_;
 private:
//  nlohmann::json j_suggestions;
//  boost::asio::ip::tcp::socket serv_socket;
//  boost::asio::io_context io;
};

#endif // INCLUDE_HEADER_HPP_
