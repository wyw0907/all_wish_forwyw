#ifndef WISH_TCP_CLIENT_H
#define WISH_TCP_CLIENT_H
#include "tcp_session.h"
#include "../session_pool.h"

namespace wish
{
    template <typename Session, typename = typename std::enable_if_t<std::is_base_of_v<tcp_session, Session>>>
    class client : public session_pool<Session>
    {
    public:
        using Family = asio::ip::tcp;
        using super = session_pool<Session>;
        client() : super(), m_resolver(*(this->m_io_context))
        {
        }

        bool connect(const std::string &ip, uint16_t port)
        {
            m_session_ref = this->add_object();
            std::shared_ptr<Session> ss = m_session_ref.lock();
            if (ss == nullptr)
            {
                return false;
            }
            Family::endpoint ep;
            auto rets = m_resolver.resolve(ip, std::to_string(port));
            for (auto it = rets.begin(); it != rets.end(); ++it)
            {
                ep = it->endpoint();
                break;
            }
            std::cout << "connecting " << ep.address().to_string() << ":" << ep.port() << std::endl;
            std::condition_variable cond;
            std::mutex mtx;
            std::unique_lock<std::mutex> lck(mtx);
            ss->socket().async_connect(ep, [this, ss, &cond, &mtx](const asio::error_code &ec) {
                if (ec)
                {
                    ss->connect_fail(ec);
                    this->del_object(ss->id());
                }
                else
                {
                    ss->connect();
                }
                cond.notify_all();
            });
            this->run();
            cond.wait_for(lck, std::chrono::seconds(5));
            return ss->is_connected();
        }

        bool send_msg(const std::string &message)
        {
            return send_msg(message.data(), message.length());
        }

        bool send_msg(const char *data, size_t size)
        {
            auto ss = m_session_ref.lock();
            if (ss == nullptr || !ss->is_connected())
            {
                return false;
            }
            ss->send_msg(data, size);
            return true;
        }

    protected:
        std::weak_ptr<Session> m_session_ref;
        Family::resolver m_resolver;
    };

    using simple_client = client<simple_session>;
} // namespace wish

#endif