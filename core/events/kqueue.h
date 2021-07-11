#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include <stdio.h>
#include "../log/log.h"
class EventLoop
{
public:
    bool start()
    {
        m_fd = kqueue();
        if (m_fd == -1)
        {
            LOG_FAT("init kqueue fail %s(%d)", strerror(errno), errno);
            return false;
        }
        EV_SET(&m_ke, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);
        EV_SET(&m_ke, STDOUT_FILENO, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        
        if (kevent(m_fd, &m_ke, 1, NULL, 0, NULL) == -1)
        {
            LOG_FAT("kevent set fail %s(%d)", strerror(errno), errno);
            return false;
        }

        while (true)
        {
            try 
            {
                loop();
            }
            catch (const std::exception &e)
            {
                LOG_ERR("EventLoop excetion %s\n", e.what());
            }
        }
    }

protected:
    virtual void on_timer()
    {

    }

    virtual void on_read(const char *data, size_t len)
    {

    }

    void loop()
    {
        int retval, numevent = 0;
        struct timespec timeout;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 100'000'000; // 100ms
        
        retval = kevent(m_fd, NULL, 0, &m_ke, 2, &timeout);
        if (retval > 0)
        {
            for (int idx = 0; idx < retval; ++idx)
            {
                int mask = 0;
                struct kevent *e = &m_ke + idx;
                if (e->filter == EVFILT_READ)
                {
                    char buf[1024];
                    int read_len = read(e->ident, buf, 1024);
                }
                if (e->filter == EVFILT_WRITE)
                {
                    LOG_DBG("kqueue write");
                }
            }
        }
        else
        {
            on_timer();
        }
    }

private:
    int m_fd = -1;
    struct kevent m_ke;
};
