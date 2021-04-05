#include <SuggestServer.hpp>

int main(int argc, char* argv[]){
  if(argc != 4){
    std::cerr << "Error! Usage:" << std::endl
              << "    demo <request_path> <ip address> <port>" << std::endl
              << "Example:" << std::endl
              << "    demo /v1/api/suggest 0.0.0.0 3000" << std::endl;
    return 1;
  }

  unsigned port = static_cast<unsigned>(std::atoi(argv[3]));
  boost::string_view url {argv[1]};
  boost::asio::ip::address address = boost::asio::ip::make_address(argv[2]);

  SuggestServer server(url, address, port);
  server.StartServer();
}