#include <asio.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <random>
using asio::ip::tcp;
constexpr int max_length = 1024;
int main(int argc, char **argv)
{
    asio::io_context _ctx;
    tcp::socket s(_ctx);
    tcp::resolver _rs(_ctx);
    asio::connect(s, _rs.resolve(argv[1], argv[2]));
    char reply[max_length] = {0};
    s.async_read_some(asio::buffer(reply, max_length),
                     [&reply, &s](std::error_code ec, std::size_t length) {
                         std::cout << "async_read" << std::endl;
                         if (!ec)
                         {
                             std::cout << reply << std::endl;
                             asio::write(s, asio::buffer(reply, length));
                         }
                         else
                         {
                             std::cout << ec.message() << std::endl;
                             exit(1);
                         }
                     });
    std::string msg;
    std::cin >> msg;

    asio::write(s, asio::buffer(msg, msg.size()));

    //auto size = asio::read(s, asio::buffer(reply, sizeof(reply)));

    _ctx.run();
}