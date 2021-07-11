#include "nn_socket.h"

namespace wish {
    namespace nn {

        socket::socket(int domain, protocol p, int buffer_size) : m_buf(buffer_size) 
        {
            m_sock = nn_socket(domain, int(p));
            if (m_sock < 0)
            {
                on_error("nn_socoket");
            }
        }

        socket::~socket() {}

        void socket::close()
        {
            int rc = nn_close(m_sock);
            if (rc != 0)
            {
                on_error("nn_close");
            }
        }

        int socket::send(const std::string& msg, int flags)
        {
            int rc = nn_send(m_sock, msg.c_str(), msg.length(), flags);
            if (rc < 0 || nn_errno() != EAGAIN)
            {
                on_error("send");
            }
            return rc;
        }

        int socket::recv(int flags)
        {
            int rc = nn_recv(m_sock, m_buf.data(), m_buf.size(), flags);
            if (rc < 0)
            {
                switch (nn_errno())
                {
                case ETIMEDOUT:
                case EAGAIN:
                    break;
                case EINTR:
                    LOG_WRN("interrupted when receiveng from %s", m_url.c_str());
                    break;          
                default:
                    on_error("nn_recv");
                }
                m_message.clear();
                return rc;
            }
            else
            {
                m_message.assign(m_buf.data(), rc);
            }  
            return rc;
        }

        const std::string & socket::last_message() const
        {
            return m_message;
        }

        void socket::setsockopt(int level, int option, const void *optval, size_t optvallen)
        {
            int rc = nn_setsockopt(m_sock, level, option, optval, optvallen);
            if (rc != 0)
            {
                on_error("nn_setsockopt");
            }
        }

        void socket::setsockopt(int level, int option, const std::string & value)
        {
            setsockopt(level, option, value.c_str(), value.size());
        }

        void socket::setsockopt(int level, int option, int value)
        {
            setsockopt(level, option, &value, sizeof(value));
        }

        void socket::getsockopt(int level, int option, void *optval, size_t *optvallen)
        {
            int rc = nn_getsockopt(m_sock, level, option, optval, optvallen);
            if (rc != 0)
            {
                on_error("getsockopt");
            }
        }

        int  socket::getsockopt(int level, int option)
        {
            int rc;
            size_t s = sizeof(rc);
            getsockopt(level, option, &rc, &s);
            return rc;
        }

        void socket::on_error(const std::string &msg)
        {
            int no = nn_errno();
            switch (no)
            {
                case EINTR:
                case EAGAIN:
                    break;
                case ETIMEDOUT:
                default:
                    LOG_ERR("%s : %s(%d)", msg.c_str(), nn_strerror(no), no);
                    throw std::runtime_error(msg.c_str());
            }
        }

        nn::protocol socket::get_protocol() const
        {
            return m_protocol;
        }

        server::server(protocol p) : server(AF_SP, p) {}

        server::server(int domain, protocol p) : server(domain, p, MAX_MSG_LENGTH) {}

        server::server(int domain, protocol p, int buffer_size) : socket(domain, p, buffer_size) {}

        server::~server() {}

        void server::bind(transport t, const std::string &url /* or path*/)
        {
            m_url = to_string(t) + url;
            int rc = nn_bind(m_sock, m_url.c_str());
            if (rc < 0)
            {
                on_error("nn_bind");
            }
        }

        client::client(protocol p) : client(AF_SP, p) {}

        client::client(int domain, protocol p) : client(domain, p, MAX_MSG_LENGTH) {}

        client::client(int domain, protocol p, int buffer_size) : socket(domain, p, buffer_size) {}

        client::~client() {}

        void client::connect(transport t, const std::string &url /* or path*/)
        {
            m_url = to_string(t) + url;
            int rc = nn_connect(m_sock, m_url.c_str());
            if (rc < 0)
            {
                on_error("nn_connect");
            }
        }
    }

}
