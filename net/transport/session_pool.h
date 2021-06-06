#ifndef WISH_SESSION_POOL_H
#define WISH_SESSION_POOL_H
#include <map>
#include "../util/spin_lock.h"
#include "session.h"
namespace wish
{
    template <typename Session,
              typename Matrix>
    class session_pool : public std::enable_shared_from_this<session_pool<Session, Matrix> >, public Matrix
    {
    public:
        using SessionType = std::shared_ptr<Session>;
        session_pool()
            : m_io_context(std::make_shared<asio::io_context>(2)),
              m_ev_strand(*m_io_context)
        {
        }

        virtual ~session_pool()
        {
            this->m_io_context->stop();
            m_context.join();
        }

        void run()
        {
            m_context = std::thread([this]() {
                try
                {
                    this->m_io_context->run();
                }
                catch (const asio::error_code &ec)
                {
                    std::cout << "io error : " << ec.message() << std::endl;
                    raise(ec.value());
                }
            });
        }

        void wait()
        {
            std::mutex mtx;
            std::condition_variable cv;
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck);
        }

        SessionType create_object()
        {
            std::unique_lock<spin_lock> lck(m_lock);
            uint64_t id = m_sequence_id.load();
            m_sequence_id++;
            lck.unlock();
            return std::make_shared<Session>(id, m_io_context, this);
        }

        SessionType add_object()
        {
            auto ss = create_object();
            return add_object(ss) ? ss : nullptr;
        }

        bool add_object(SessionType ss)
        {
            std::lock_guard<spin_lock> lck(m_lock);
            if (m_objects.count(ss->id()) != 0)
            {
                return false;
            }
            m_objects.insert({ss->id(), ss});
            return true;
        }

        void del_object(uint64_t id)
        {
            std::lock_guard<spin_lock> lck(m_lock);
            auto it = m_objects.find(id);
            if (it != m_objects.end())
            {
                m_objects.erase(it);
            }
        }

        void for_each(std::function<void(SessionType)> func)
        {
            std::lock_guard<spin_lock> lck(m_lock);
            for (const auto &v : m_objects)
            {
                func(v.second);
            }
        }

        void clear()
        {
            std::lock_guard<spin_lock> lck(m_lock);
            m_objects.clear();
        }

        size_t size()
        {
            std::lock_guard<spin_lock> lck(m_lock);
            return m_objects.size();
        }

        SessionType find(uint64_t id)
        {
            std::lock_guard<spin_lock> lck(m_lock);
            auto it = m_objects.find(id);
            if (it != m_objects.end())
            {
                return it->second;
            }
            return nullptr;
        }

    protected:
        std::shared_ptr<asio::io_context> m_io_context;
        asio::io_context::strand m_ev_strand;

    private:
        std::atomic<uint64_t> m_sequence_id = 0;
        spin_lock m_lock;
        std::map<uint64_t, SessionType> m_objects;
        std::thread m_context;
    };
} // namespace wish
#endif