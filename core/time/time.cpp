
#include "time.h"

namespace wish {

uint64_t time_util::timestamp()
{
    time_t t;
    time(&t);
    return t;
}

uint64_t time_util::mtimestamp()
{
    return time_util::utimestamp() / 1000;
}

uint64_t time_util::utimestamp()
{
    auto m = std::chrono::system_clock::now().time_since_epoch();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(m).count();
    return diff;
}

uint64_t time_util::ntimestamp()
{
    auto m = std::chrono::system_clock::now().time_since_epoch();
    auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(m).count();
    return diff;
}

std::string time_util::current_time(const char *fmt)
{
    time_t t;
    time(&t);
    char sz_time[32] = {0};
    strftime(sz_time, 20, fmt, localtime(&t));
    return sz_time;
}

std::string time_util::current_time()
{   
    std::string ts = current_time("%Y-%m-%d %H:%M:%S");

    char ret_time[64] = {0};
    snprintf(ret_time, sizeof(ret_time), "%s.%06d", ts.c_str(), int(time_util::utimestamp() % 1000'000));
    return ret_time;
}

}