#ifndef WISH_TCP_SERVER_H
#define WISH_TCP_SERVER_H
#include "../session_pool.h"
#include "session.h"
#include "matrix.h"
namespace wish
{
    namespace tcp
    {
        template <typename Session,
                  typename Matrix>
        class server : public session_pool<Session, Matrix>
        {
        public:
            using Family = asio::ip::tcp;
            using super = session_pool<Session, Matrix>;
            server() : super(), m_acceptor(*(this->m_io_context))
            {
            }
            virtual ~server()
            {
                shutdown();
            }
            // ipv4 默认地址 0.0.0.0, ipv6默认地址 ::
            bool set_addr(uint16_t port, const std::string &ip = "")
            {
                if (ip.empty())
                {
                    m_server_addr = Family::endpoint(Family::v4(), port);
                }
                else
                {
                    asio::error_code ec;
                    auto addr = asio::ip::make_address(ip, ec);
                    if (ec)
                    {
                        std::cout << "set_addr " << ec.message() << std::endl;
                        return false;
                    }
                    m_server_addr = Family::endpoint(addr, port);
                }
                return true;
            }

            void set_sockopt_reuse()
            {
                asio::error_code ec;
                m_acceptor.set_option(Family::acceptor::reuse_address(true), ec);
            }

            bool start()
            {
                asio::error_code ec;
                if (!m_acceptor.is_open())
                {
                    m_acceptor.open(m_server_addr.protocol(), ec);
                    if (ec)
                    {
                        std::cout << "acceptor " << ec.message() << std::endl;
                        return false;
                    }
                }

                m_acceptor.bind(m_server_addr);
                if (ec)
                {
                    std::cout << "bind " << ec.message() << std::endl;
                    return false;
                }
                m_acceptor.listen(Family::acceptor::max_listen_connections, ec);
                if (ec)
                {
                    std::cout << "listen " << ec.message() << std::endl;
                    return false;
                }
                std::cout << "listen to " << m_server_addr.address().to_string()
                          << ":" << m_server_addr.port() << std::endl;
                do_accept();
                this->run();
                return true;
            }

            bool send_msg(uint64_t sid, const std::string &message)
            {
                return send_msg(sid, message.data(), message.length());
            }

            bool send_msg(uint64_t sid, const char *data, size_t size)
            {
                auto ss = this->find(sid);
                if (ss == nullptr || !ss->is_connected())
                {
                    return false;
                }
                ss->send_msg(data, size);
                return true;
            }

            void shutdown()
            {
                m_stopped = true;
                this->for_each([] (auto&& ss) {
                    if(ss->is_connected())
                    {
                        ss->disconnect();
                    }
                });
                this->clear();
            }

            void disconnect(uint64_t sid)
            {
                auto ss = this->find(sid);
                if (ss != nullptr)
                {
                    if (!ss->is_connected())
                    {
                        throw std::logic_error("session is not connected");
                    }
                    ss->disconnect();
                    this->del_object(sid);
                }
                else
                {
                    throw std::logic_error("invalid session id");
                }
            }

            virtual void on_receive_error(uint64_t sid, const asio::error_code &ec) override
            {
                // 异步删除session
                this->disconnect(sid);
            }

        protected:
            void on_accept(std::shared_ptr<Session> sock)
            {
                sock->connect();
            }

            virtual void on_accept_error(const asio::error_code &ec)
            {
                std::cout << "accetor error " << ec.message() << std::endl;
            }

            void do_accept()
            {
                auto sock = this->create_object();
                m_acceptor.async_accept(sock->socket(), [this, sock](auto &&ec) {
                    if(m_stopped)
                    {
                        return;
                    }
                    if (ec)
                    {
                        this->on_accept_error(ec);
                    }
                    else
                    {
                        this->on_accept(sock);
                        this->add_object(sock);
                    }
                    do_accept();
                });
            }

        protected:
            std::atomic_bool m_stopped{false};
            Family::endpoint m_server_addr;
            Family::acceptor m_acceptor;
        };
    }
} // namespace wish
#endif