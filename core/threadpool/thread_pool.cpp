#include "thread_pool.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
namespace wish
{

    uint32_t current_thread_id()
    {
 
#ifdef __LINUX
        std::thread_local const uint32_t tid = static_cast<uint32_t>(::syscall(SYS_gettid));
        return tid;
#else
        return static_cast<uint32_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif 
    }
    thread_local std::string t_threadname;
    std::string current_thread_name()
    {
        return t_threadname;
    }

    void set_thread_name(const std::string &name)
    {
        t_threadname = name;
    }
}