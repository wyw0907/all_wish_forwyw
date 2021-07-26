#ifndef WISH_EVENT_H
#define WISH_EVENT_H
#include <mutex>
#include <condition_variable>
#include <chrono>
namespace wish
{
    class event
    {
    public:
        void set()
        {
            std::unique_lock<std::mutex> lck(m_mtx);
            // volatile 特点，写性能较差
            if (!m_setted)
            {
                m_setted = true;
            }
            m_cond.notify_one();
        }

        bool wait(std::chrono::milliseconds mseconds)
        {
            std::unique_lock<std::mutex> lck(m_mtx);
            if (m_setted)
            {
                m_setted = false;
                return true;
            }
            auto status =  m_cond.wait_for(lck, mseconds);
            if (status == std::cv_status::no_timeout)
            {
                m_setted = false;
                return true;
            }
            else if(m_setted)
            {
                m_setted = false;
                return true;
            }
            else
            {
                return false;
            }
        }
    private:
        std::mutex m_mtx;
        std::condition_variable m_cond;
        volatile bool m_setted = false;
    };
}

#endif