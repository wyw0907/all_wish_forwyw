#pragma once
#include <thread>

class Runnable 
{
public:
    Runnable()
    {

    }

    virtual ~Runnable()
    {
        m_stop = true;
        m_running.join();
    }

    virtual void start()
    {
        m_running = std::thread([this] {
            this->run();
            m_stop = true;
        });
    }

protected:
    virtual void run() = 0;

    std::atomic_bool                    m_stop{false};
    std::thread                         m_running;
};