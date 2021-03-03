
#include <asio.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using asio::ip::tcp;

class session
    : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, int id)
        : socket_(std::move(socket))
    {
        //snprintf(data_, sizeof(data_), "hello, world %d !", id);
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, max_length),
                                [this, self](std::error_code ec, std::size_t length) {
                                    std::cout << "async_read_some" << std::endl;
                                    if (!ec)
                                    {
                                        do_write(length);
                                    }
                                    else
                                    {
                                        std::cout << ec.message() << std::endl;
                                    }
                                });
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(data_, length),
                          [this, self](std::error_code ec, std::size_t len) {
                              std::cout << "async_write : " << len << std::endl;
                              if (!ec)
                              {
                                  do_read();
                              }
                              else
                              {
                                  std::cout << ec.message() << std::endl;
                              }
                          });
    }

    tcp::socket socket_;
    constexpr static int max_length = 1024;
    char data_[max_length];
};

class server
{
public:
    server(asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          socket_(io_context)
    {
    }

    void start()
    {
        return do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](std::error_code ec) {
                                   std::cout << "async_accept " << std::endl;
                                   if (!ec)
                                   {
                                       std::make_shared<session>(std::move(socket_), ++id_)->start();
                                   }
                                   else
                                   {
                                       std::cout << ec.message() << std::endl;
                                   }
                                   do_accept();
                               });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::atomic<int> id_{0};
};

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        asio::io_context io_context;

        server s(io_context, std::atoi(argv[1]));
        s.start();
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
