#ifndef WISH_LOG_H
#define WISH_LOG_H
#include <string>
#include <time.h>
#include "../common.h"
namespace wish
{
class logger
{
public:
    static void debug_out(const char *fmt, ...);
    static void info_out(const char *fmt, ...);
    static void warning_out(const char *fmt, ...);
    static void error_out(const char *fmt, ...);
    static void fatal_out(const char *fmt, ...);
    static logger &instance();

protected:
    logger() {}

    enum class level
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };
    template <level L>
    class level_msg
    {
    };

    std::string formatv(const char *fmt, va_list args);

    template <class L>
    void logv(const char *fmt, va_list args)
    {
        return log<L>(formatv(fmt, args));
    }

    template <class L, class S>
    void log(S &&content)
    {
        timespec tt;
        clock_gettime(CLOCK_REALTIME, &tt);
        char sz_time[32] = {0};
        strftime(sz_time, 20, "%Y-%m-%d %H:%M:%S", localtime(&tt.tv_sec));
        fprintf(stdout, "%s[%s.%ld][%s] %s\033[0m\n", L::color, sz_time, tt.tv_nsec / 1000'000, L::name, content.c_str());
    }
};

template <>
class logger::level_msg<logger::level::Debug>
{
public:
    const static char name[];
    const static char color[];
};

template <>
class logger::level_msg<logger::level::Info>
{
public:
    const static char name[];
    const static char color[];
};

template <>
class logger::level_msg<logger::level::Warning>
{
public:
    const static char name[];
    const static char color[];
};

template <>
class logger::level_msg<logger::level::Error>
{
public:
    const static char name[];
    const static char color[];
};

template <>
class logger::level_msg<logger::level::Fatal>
{
public:
    const static char name[];
    const static char color[];
};
} // namespace wish

#define LOG_DBG wish::logger::debug_out
#define LOG_INF wish::logger::info_out
#define LOG_WRN wish::logger::warning_out
#define LOG_ERR wish::logger::error_out
#define LOG_FAT wish::logger::fatal_out

#endif
