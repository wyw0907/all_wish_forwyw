#include "log.h"
namespace wish {

std::string logger::formatv(const char *fmt, va_list args)
{
    std::string s;
    if (fmt && *fmt)
    {
        va_list args_copy;
        va_copy(args_copy, args);
        int len = std::vsnprintf(nullptr, 0, fmt, args_copy);
        if (len > 0)
        {
            s.resize(len);
            va_copy(args_copy, args);
            std::vsprintf(const_cast<char *>(s.data()), fmt, args_copy);
        }
    }
    return s;
}

logger & logger::instance()
{
    static logger _instance;
    return _instance;
}

void logger::debug_out(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger::instance().logv<logger::level_msg<level::Debug>>(fmt, args);
    va_end(args);
}

void logger::info_out(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger::instance().logv<logger::level_msg<level::Info>>(fmt, args);
    va_end(args);
}

void logger::warning_out(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger::instance().logv<logger::level_msg<level::Warning>>(fmt, args);
    va_end(args);
}

void logger::error_out(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger::instance().logv<logger::level_msg<level::Error>>(fmt, args);
    va_end(args);
}

void logger::fatal_out(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger::instance().logv<logger::level_msg<level::Fatal>>(fmt, args);
    va_end(args);
}

const char logger::level_msg<logger::level::Debug>::name[] = "Debug";
const char logger::level_msg<logger::level::Debug>::color[] = "\033[1;32;40m";

const char logger::level_msg<logger::level::Info>::name[] = "Info";
const char logger::level_msg<logger::level::Info>::color[] = "\033[0m";

const char logger::level_msg<logger::level::Warning>::name[] = "Warning";
const char logger::level_msg<logger::level::Warning>::color[] = "\033[1;33;40m";

const char logger::level_msg<logger::level::Error>::name[] = "Error";
const char logger::level_msg<logger::level::Error>::color[] = "\033[1;31;40m";

const char logger::level_msg<logger::level::Fatal>::name[] = "Fatal";
const char logger::level_msg<logger::level::Fatal>::color[] = "\033[1;40;40m";


}