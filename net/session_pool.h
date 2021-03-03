#ifndef WISH_SESSION_POOL_H
#define WISH_SESSION_POOL_H
#include <asio.hpp>
#include <map>
#include "detail/spin_lock.h"
namespace wish
{
    template <typename Session>
    class session_pool
    {
    public:
        using SessionType = std::shared_ptr<Session>;
        session_pool()
        {
            m_io_context = std::make_shared<asio::io_context>(2);
        }

        void run()
        {
            std::thread([this]() { 
                this->m_io_context->run();
            }).detach();
        }

        void wait()
        {
            std::mutex mtx;
            std::condition_variable cv;
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck);
        }

        SessionType add_object()
        {
            std::unique_lock<spin_lock> lck(m_lock);
            uint64_t id = m_sequence_id.load();
            m_sequence_id ++;
            lck.unlock();
            auto ss = std::make_shared<Session>(id, m_io_context);
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

    private:
        std::atomic<uint64_t> m_sequence_id = 0;
        spin_lock m_lock;
        std::map<uint64_t, SessionType> m_objects;
    };
} // namespace wish
#endif