#include <SuggestServer.hpp>
//#include <beast.hpp>

void suggest_handler(boost::asio::ip::tcp::socket& socket){
  beast::flat_buffer buffer;
  beast::error_code ec;
  beast::http::request<beast::http::string_body> req;
  for(;;){
    beast::http::read(socket,buffer, req, ec);
    if(ec == beast::http::error::end_of_stream)
      break;
    if (ec)
      return fail(ec, "read");


  }
  beast::http::response<beast::http::string_body> res;
  res.body = req.body;
  beast::http::response_serializer<beast::http::string_body> rs{res};
  beast::http::write(socket, rs);
//  for(;;){std::cout<< "Im WORKING" << std::endl;}

}
//---------------------------------------------------------------------------------------------
//int main(int argc, char* argv[])
//{
//  // Check command line arguments.
//  if (argc != 4) {
//    std::cerr << "Usage: demo <URL> <address> <port>\n"
//              << "Example:\n"
//              << "    demo  /v1/api/suggest 0.0.0.0 8080 .\n";
//    return EXIT_FAILURE;
//  }
//  boost::string_view URL (argv[1]);
//  auto const address = boost::asio::ip::make_address(argv[2]);
//  auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
//
//}
//----------------------------------------------------------------------------------------------
int main() {
  try {
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), 8080);
    boost::asio::ip::tcp::acceptor acceptor(ioService, endPoint);

    for (;;) {
      boost::asio::ip::tcp::socket socket(ioService);
      acceptor.accept(socket);
      std::thread(std::bind(&suggest_handler, std::move(socket))).detach();
    }
  }
//      char buf[128] = {0};
//      boost::system::error_code ec;
//      size_t readBytes = socket1.read_some(boost::asio::buffer(buf), ec);
//      std::string myString = buf;
//      boost::system::error_code ignored_error;
//      boost::asio::write(socket1, boost::asio::buffer(myString), ignored_error);
//    }
//  }
  catch(std::exception& e){
    std::cerr << e.what() << std::endl;
  }

//  boost::asio::io_service io;
//  printer p(io);
//  int count = 0;
//  boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
//  t.async_wait(std::bind(print, std::placeholders::_1, &t, &count));
//  io.run();
  return 0;
}