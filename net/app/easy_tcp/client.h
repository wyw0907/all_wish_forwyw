#include "net/transport/tcp/client.h"
#include <thread>
#include <condition_variable>
namespace wish
{
    namespace easy_tcp
    {
        using recv_cb_type = std::function<void(const char *data, size_t size)>;
        class client : public wish::tcp::client<wish::tcp::session<wish::tcp::matrix>, wish::tcp::matrix>
        {
        public:
            void regist_recv_cb(recv_cb_type cb)
            {
                m_recv_cb = cb;
            }

            bool sync_connect(const std::string &ip, uint16_t port, size_t timeout = 15)
            {
                std::unique_lock<std::mutex> lck(m_mtx);
                if(!this->connect(ip, port))
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
                if (m_recv_cb)
                {
                    m_recv_cb(data, size);
                }
            }
            void on_connect(uint64_t id) override
            {
                std::unique_lock<std::mutex> lck(m_mtx);
                m_cv.notify_all();
            }
            void on_disconnect(uint64_t id) override
            {
                std::unique_lock<std::mutex> lck(m_mtx);
                m_cv.notify_all();
            }
            void on_connect_fail(uint64_t id) override
            {
                std::unique_lock<std::mutex> lck(m_mtx);
                m_cv.notify_all();
            }

        private:
            recv_cb_type m_recv_cb;
            std::condition_variable m_cv;
            std::mutex m_mtx;
        };
    }
}