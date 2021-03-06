#ifndef WISH_TCP_SESSION_H
#define WISH_TCP_SESSION_H
#include "../session.h"
#include "matrix.h"
namespace wish
{
    namespace tcp
    {
        template <typename Matrix, 
                  typename = typename std::enable_if_t<std::is_base_of_v<tcp::matrix, Matrix>>>
        class session : public wish::session<asio::ip::tcp::socket, Matrix>
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

            using super = wish::session<asio::ip::tcp::socket, Matrix>;
            using super::super;

            bool is_connected()
            {
                return m_status == link_status::Connected;
            }

            virtual void on_connect()
            {
                this->m_matrix->on_connect(this->m_id);
            }

            virtual void on_connect_fail(const std::error_code &ec)
            {
                this->m_matrix->on_connect_fail(this->m_id);
            }

            virtual void on_disconnect()
            {
                this->m_matrix->on_disconnect(this->m_id);
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
                this->stop();
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
    }
} // namespace wish

#endif