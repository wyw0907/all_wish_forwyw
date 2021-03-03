#ifndef WISH_TCP_SERVER_H
#define WISH_TCP_SERVER_H
#include "../session_pool.h"
#include "tcp_session.h"
namespace wish
{
    template <typename Session, typename = typename std::enable_if_t<std::is_base_of_v<tcp_session, Session>>>
    class server : public session_pool<Session>
    {
    public:
        using Family = asio::ip::tcp;
        using super = session_pool<Session>;
        server() : super(), m_acceptor(*(this->m_io_context))
        {

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
            if(ec)
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

    protected:
        void on_accept(std::shared_ptr<tcp_session> sock)
        {
            sock->connect();
        }

        virtual void on_accept_error(const asio::error_code& ec)
        {
            std::cout << "accetor error " << ec.message() << std::endl;
        }

        void do_accept()
        {
            auto sock = this->add_object();
            m_acceptor.async_accept(sock->socket(), [this, sock] (auto&& ec) {
                if (ec)
                {
                    this->on_accept_error(ec);
                    this->del_object(sock->id());
                }
                else
                {
                    this->on_accept(sock);
                }
                do_accept();      
            });
            
        }
    protected:
        Family::endpoint m_server_addr;
        Family::acceptor m_acceptor;
    };
    using simple_server = server<simple_session>;
} // namespace wish
#endif