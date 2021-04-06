// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_SUGGESTSERVER_HPP_
#define INCLUDE_SUGGESTSERVER_HPP_

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <thread>
#include <string>
#include <iostream>
#include <vector>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace ip = boost::asio::ip;
using json = nlohmann::json;

struct Suggestion{
  std::string str_name;
  unsigned cost;
};


class SuggestServer{
 public:
  explicit SuggestServer(boost::string_view& URL,
                         ip::address& address,
                         unsigned port):  URL_(URL),
                                          address_(address),
                                          port_(port){};
  [[noreturn]] void StartServer();
  void RequestHandler(ip::tcp::socket&& socket);

 private:
  void UpdateData();
  std::vector<Suggestion> GetMatches(std::string& req);
  json GetResponse(std::string& req);
 private:
  boost::string_view URL_;
  ip::address address_;
  unsigned port_;
  std::vector<Suggestion> v_data_;
  std::mutex mutex_;
};

#endif // INCLUDE_SUGGESTSERVER_HPP_
