#ifndef WISH_SESSION_H
#define WISH_SESSION_H
#include "matrix.h"
#include <iostream>
namespace wish
{
    template <typename Socket,
              typename Matrix,
              typename = typename std::enable_if_t<std::is_base_of_v<i_matrix, Matrix> > >
    class session : public std::enable_shared_from_this<session<Socket, Matrix> >
    {
    public:
        session(uint64_t id, std::shared_ptr<asio::io_context> io_ctx, Matrix *matrix)
            : m_id(id),
              m_socket(*io_ctx),
              m_rw_strand(*io_ctx),
              m_matrix(matrix)
        {
        }
        virtual ~session()
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

        void stop()
        {
            m_stopped = true;
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
            m_matrix->on_send_error(m_id, ec);
        }

        virtual void on_send_msg(size_t length)
        {
            m_matrix->on_send_success(m_id, length);
        }

        virtual void on_recv_error(const std::error_code &ec)
        {
            m_matrix->on_receive_error(m_id, ec);
        }

        virtual void on_recv_msg(const char *msg, size_t length)
        {
            m_matrix->on_receive(m_id, msg, length);
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
            asio::async_write(m_socket, sending_buf, asio::transfer_exactly(10),
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
                                         if (m_stopped)
                                         {
                                             return;
                                         }
                                         if (ec)
                                         {
                                             if (ec.value())
                                                 on_recv_error(ec);
                                         }
                                         else
                                         {
                                             on_recv_msg(m_recv_buffer.data(), length);
                                         }
                                         if (!m_stopped)
                                         {
                                             do_recv_msg();
                                         }
                                     });
        }

    protected:
        std::atomic_bool m_stopped{false};
        uint64_t m_id;
        Socket m_socket;
        asio::io_context::strand m_rw_strand;
        std::array<char, 1024> m_recv_buffer;
        Matrix *m_matrix;
    };
} // namespace wish

#endif