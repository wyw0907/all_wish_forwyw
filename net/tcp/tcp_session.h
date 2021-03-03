#ifndef WISH_TCP_SESSION_H
#define WISH_TCP_SESSION_H
#include "../session.h"

namespace wish
{
    class tcp_session : public session<asio::ip::tcp::socket>
    {
    public:
        using Family = asio::ip::tcp;
        enum class link_status
        {
            Invalid,
            Connecting,
            Connected,
            ShuttingDown,
            Broken,
            Connectfail,
        };

        using super = session<asio::ip::tcp::socket>;
        using super::super;

        bool is_connected()
        {
            return m_status == link_status::Connected;
        }

        virtual void on_connect()
        {
            std::cout << "connected" << std::endl;
        }

        virtual void on_connect_fail(const std::error_code &ec)
        {
            std::cout << "connect fail: " << ec.message() << std::endl;
        }
        virtual void on_disconnect()
        {
        }

        virtual void on_recv_msg(const char *msg, size_t length) override
        {
            std::cout << "recv msg: " << std::string(msg, length) << std::endl;
            //send_msg(std::string("i recv").data(), 6);
        }

        virtual void on_recv_error(const std::error_code &ec) override
        {
            std::cout << "recv error: " << ec.message() << std::endl;
            m_status = link_status::Broken;
        }

        void connect()
        {
            m_status = link_status::Connected;
            this->start();
            on_connect();
        }

        void disconnect()
        {
            m_status = link_status::Broken;
            on_disconnect();
        }

        void connect_fail(const std::error_code &ec)
        {
            m_status = link_status::Connectfail;
            on_connect_fail(ec);
        }

    protected:
        link_status m_status = link_status::Invalid;
    };

    class simple_session : public tcp_session
    {
    public:
        using tcp_session::tcp_session;
        using Handle = std::function<void(uint64_t, const char *, size_t)>;
        static void handle_msg(Handle func)
        {
            simple_session::handler = std::make_shared<Handle>(func);
        }
        virtual void on_recv_msg(const char *msg, size_t length) override
        {
            if (simple_session::handler != nullptr)
            {
                (*simple_session::handler)(this->id(), msg, length);
            }
        }
    private:
        static std::shared_ptr<Handle> handler;
    };
    std::shared_ptr<simple_session::Handle> simple_session::handler =nullptr;
} // namespace wish

#endif