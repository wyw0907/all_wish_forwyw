#ifndef WISH_TCP_CLIENT_H
#define WISH_TCP_CLIENT_H
#include "../session_pool.h"
#include "session.h"
#include "matrix.h"
namespace wish
{
    namespace tcp
    {
        template <typename Session,
                  typename Matrix>
        class client : public session_pool<Session, Matrix>
        {
        public:
            using Family = asio::ip::tcp;
            using super = session_pool<Session, Matrix>;
            client() : super(), m_resolver(*(this->m_io_context))
            {
            }

            virtual ~client()
            {
                shutdown();
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
                ss->socket().async_connect(ep, [this, ss](const asio::error_code &ec) {
                    if (ec)
                    {
                        ss->connect_fail(ec);
                        this->del_object(ss->id());
                    }
                    else
                    {
                        ss->connect();
                    }
                });
                this->run();
                return true;
            }

            void shutdown()
            {
                auto ss = m_session_ref.lock();
                if (ss == nullptr)
                {
                    assert(false);
                }
                if (ss->is_connected())
                {
                    ss->disconnect();
                }
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

    }
} // namespace wish

#endif