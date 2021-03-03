#ifndef WISH_SESSION_H
#define WISH_SESSION_H
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <iostream>
namespace wish
{
    template <typename Socket>
    class session : public std::enable_shared_from_this<session<Socket>>
    {
    public:
        session(uint64_t id, std::shared_ptr<asio::io_context> io_ctx) : m_id(id),
                                                                         m_socket(*io_ctx),
                                                                         m_rw_strand(*io_ctx)
        {
        }

        uint64_t id() const
        {
            return m_id;
        }

        void start()
        {
            recv_msg();
        }

        void send_msg(const char *data, size_t size)
        {
            asio::const_buffer sending_buf(data, size);
            m_rw_strand.dispatch([sending_buf = std::move(sending_buf), this]() mutable {
                this->do_send_msg(std::move(sending_buf));
            });
        }

        void recv_msg()
        {
            m_rw_strand.dispatch([this]() {
                this->do_recv_msg();
            });
        }

        virtual void on_send_error(const std::error_code &ec)
        {
            std::cout << "send error: " << ec.message() << std::endl;
        }

        virtual void on_send_msg(size_t length)
        {
            std::cout << "send msg : " << length << std::endl;
        }

        virtual void on_recv_error(const std::error_code &ec)
        {
            std::cout << "recv error : " << ec.message() << std::endl;
        }

        virtual void on_recv_msg(const char *msg, size_t length)
        {
            std::cout << "recv msg: " << std::string(msg, length) << std::endl;
        }

        const Socket &socket() const
        {
            return m_socket;
        }

        Socket &socket()
        {
            return m_socket;
        }

    private:
        void do_send_msg(asio::const_buffer &&sending_buf)
        {
            asio::async_write(m_socket, sending_buf,
                              [this](std::error_code ec, std::size_t length) {
                                  if (ec)
                                  {
                                      on_send_error(ec);
                                  }
                                  else
                                  {
                                      on_send_msg(length);
                                  }
                              });
        }

        void do_recv_msg()
        {
            m_socket.async_read_some(asio::buffer(&m_recv_buffer[0], m_recv_buffer.size()),
                             [this](std::error_code ec, std::size_t length) {
                                 if (ec)
                                 {
                                     if (ec.value() )
                                     on_recv_error(ec);
                                 }
                                 else
                                 {
                                     on_recv_msg(m_recv_buffer.data(), length);
                                 }
                                 do_recv_msg();
                             });
        }

    private:
        uint64_t m_id;
        Socket m_socket;
        asio::io_context::strand m_rw_strand;
        std::array<char, 1024> m_recv_buffer;
    };
} // namespace wish

#endif