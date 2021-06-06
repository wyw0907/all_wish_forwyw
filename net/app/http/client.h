#ifndef WISH_HTTP_CLIENT_H
#define WISH_HTTP_CLIENT_H
#include "../../transport/tcp/client.h"
#include "uri.h"
#include "message.h"
#include "struct.h"
#include <sstream>
namespace wish
{
    namespace http
    {
        class client : public wish::tcp::client<wish::tcp::session<wish::tcp::matrix>, wish::tcp::matrix>
        {
        public:
            wish::http::response get(const std::string &url, const header_type &headers)
            {
                return request("GET", url, headers, "");
            }

            wish::http::response put(const std::string &url, const header_type &headers, const std::string &body)
            {
                return request("PUT", url, headers, body);
            }

            wish::http::response request(const std::string &method, const std::string &url, const header_type &headers, const std::string &body)
            {
                wish::http::uri _uri;
                _uri.parse_from_array(url.c_str());
                wish::http::request _req;
                _req.version = "1.1";
                _req.method = method;
                _req.url = _uri.url();
                _req.headers = headers;
                _req.body = body;
                return request(_uri.host(), _uri.port(), _req);
            }

            wish::http::response request(const std::string &ip, uint16_t port, wish::http::request &req)
            {
                auto ss = m_session_ref.lock();
                if (ss == nullptr || !ss->is_connected())
                {
                    if (!sync_connect(ip, port))
                    {
                        throw std::runtime_error("cannot connect to host");
                    }
                }
                if (req.version.empty())
                {
                    req.version = "1.1";
                }
                auto it = req.headers.find("Content-Length");
                if (req.headers.end() == it)
                {
                    req.headers.insert({"Content-Length", std::to_string(req.body.size())});
                }
                std::stringstream stream;
                stream << req.method
                       << " "
                       << req.url
                       << " HTTP/"
                       << req.version
                       << "\r\n";
                for (auto &head : req.headers)
                {
                    stream << head.first << ": " << head.second << "\r\n";
                }
                stream << "\r\n";
                if (!req.body.empty())
                {
                    stream << req.body;
                }
                auto str = stream.str();
                DLOG("stream: {}", str);
                std::unique_lock<std::mutex> lck(m_mtx);
                this->send_msg(str);
                auto ret = m_cv.wait_for(lck, std::chrono::seconds(15));
                if (ret != std::cv_status::timeout)
                {
                    return m_rsp;
                }
                else
                {
                    throw std::runtime_error("timeout");
                }
            }

            bool sync_connect(const std::string &ip, uint16_t port, size_t timeout = 15)
            {
                std::unique_lock<std::mutex> lck(m_mtx);
                if (!this->connect(ip, port))
                {
                    return false;
                }
                m_cv.wait_for(lck, std::chrono::seconds(timeout));
                auto ss = m_session_ref.lock();
                return (ss != nullptr && ss->is_connected());
            }

        protected:
            void on_receive(uint64_t id, const char *data, size_t size) override
            {
                //DLOG("receive: {}", std::string(data, size));
                if (!m_frame.parse_from_array(data, size))
                {
                    ELOG("http parse error: {}", std::string(data, size));
                    return;
                }
                if (m_frame.is_completed())
                {
                    m_rsp.version = m_frame.version();
                    m_rsp.status = m_frame.status();
                    m_rsp.headers = m_frame.headers();
                    m_rsp.body = m_frame.body();
                    std::unique_lock<std::mutex> lck(m_mtx);
                    m_cv.notify_all();
                }
            }
            void on_connect(uint64_t id) override
            {
                DLOG("on_connect");
                std::unique_lock<std::mutex> lck(m_mtx);
                m_cv.notify_all();
            }
            void on_disconnect(uint64_t id) override
            {
                DLOG("on_disconnect");
                std::unique_lock<std::mutex> lck(m_mtx);
                m_cv.notify_all();
            }
            void on_connect_fail(uint64_t id) override
            {
                DLOG("on_connect_fail");
                std::unique_lock<std::mutex> lck(m_mtx);
                m_cv.notify_all();
            }

        private:
            std::condition_variable m_cv;
            std::mutex m_mtx;
            wish::http::response m_rsp;
            wish::http::message m_frame{HTTP_RESPONSE};
        };
    }
}

#endif