// Copyright 2020 Your Name <your_email>

#include <SuggestServer.hpp>
#include <fstream>
#include <iostream>

template <bool isRequest, class Body, class Fields, class Stream>
void SendResponse(Stream& stream,
                  beast::http::message<isRequest, Body, Fields>&& msg){
  beast::http::serializer<isRequest, Body, Fields> serializer(msg);
  beast::http::write(stream, serializer);
}

bool operator<(const Suggestion& lhs, const Suggestion& rhs) {
  return lhs.cost < rhs.cost;
}
bool operator==(const Suggestion& lhs, const Suggestion& rhs) {
  return lhs.cost == rhs.cost;
}
bool operator>(const Suggestion& lhs, const Suggestion& rhs) {
  return lhs.cost > rhs.cost;
}


void SuggestServer::RequestHandler(ip::tcp::socket&& socket) {
  beast::error_code ec;
  beast::flat_buffer buffer;

  for(;;){
    beast::http::request<beast::http::string_body> req;
    beast::http::read(socket, buffer, req, ec);

    std::cout << "Request read:" << std::endl;

    if (ec == beast::http::error::end_of_stream)
      break;

    std::cout << "1" << std::endl;
    std::cout << req.body() << std::endl;
    std::cout << "2" << std::endl;


    if ((req.method() != beast::http::verb::post) || (req.target() != URL_)
        || ec){
      beast::http::response<beast::http::string_body> res{
        beast::http::status::internal_server_error, req.version()};
      res.set(beast::http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set(beast::http::field::content_type, "text/html");
      res.keep_alive(req.keep_alive());
      res.body() = "Error! Incorrect request";
      res.prepare_payload();
      SendResponse(socket, std::move(res));
      std::cout << "Bad response sent" << std::endl;
      break;
    } else{
      beast::http::response<beast::http::string_body> res{
          beast::http::status::ok, req.version()};
      res.set(beast::http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set(beast::http::field::content_type, "application/json");
      res.keep_alive(req.keep_alive());
      std::string reqBody;
      std::string resBody;
      try{
          reqBody = json::parse(req.body()).at("input");
          resBody = GetResponse(reqBody).dump();
      }
      catch (...) {
          resBody = R"({ "status": "ill-formated request" })";
      }
      res.body() = resBody;
      res.prepare_payload();
      SendResponse(socket, std::move(res));
      std::cout << "Norm response sent" << std::endl;
    }
    if(ec)
      std::cout << "Foooo" << std::endl;
    break;

  }

  socket.shutdown(ip::tcp::socket::shutdown_send);
  std::cout << "Session success" << std::endl;

}

[[noreturn]] void SuggestServer::StartServer() {
  try {
    std::thread([this]() {
      for (;;) {
        UpdateData();
        std::this_thread::sleep_for(std::chrono::minutes(15));
      }
    }).detach();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  boost::asio::io_context ioContext;
  ip::tcp::endpoint endPoint(address_, port_);
  ip::tcp::acceptor acceptor(ioContext, endPoint);

  for(;;){
    ip::tcp::socket socket1{ioContext};
    acceptor.accept(socket1);
    std::cout << "CONNECTED" << std::endl;
    std::thread([this](ip::tcp::socket&& socket){
      RequestHandler(std::move(std::forward<ip::tcp::socket>(socket)));
    }, std::move(socket1)).detach();
  }
}
void SuggestServer::UpdateData() {
  std::ifstream data_file;
  data_file.open("../Suggest/suggestions.json");

  if(!data_file.is_open())
    throw std::runtime_error("Database file couldn't be opened");


  json j_data;
  data_file >> j_data;

  mutex_.lock();
  v_data_.clear();
  for(auto& x : j_data){
    v_data_.push_back({x.at("name"), x.at("cost")});
  }
  std::sort(v_data_.begin(), v_data_.end());
  mutex_.unlock();

  data_file.close();
}
std::vector<Suggestion> SuggestServer::GetMatches(std::string& req) {
  std::vector<Suggestion> trueSuggestions;
  mutex_.lock();
  for(auto& x : v_data_){
    if((x.str_name.size() >= req.size())
        && x.str_name.substr(0, req.size()) == req){
      trueSuggestions.push_back(x);
    }
  }
  mutex_.unlock();
  return trueSuggestions;
}
json SuggestServer::GetResponse(std::string& req) {
  json responseBody;
  std::vector<Suggestion> matches = GetMatches(req);
  unsigned index = 0;
  for(auto& x : matches){
    responseBody["suggestions"].push_back({{"text", x.str_name},
                                           {"position", index}});
    ++index;
  }
  return responseBody;
}
