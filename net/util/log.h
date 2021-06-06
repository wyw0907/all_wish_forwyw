#ifndef WISH_LOG_H
#define WISH_LOG_H
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <unistd.h>
#include <signal.h>
namespace wish
{
    enum class LogLevel
    {
        kDebug,
        kInfo,
        kWarning,
        kError,
        kFatal
    };

    namespace
    {
        spdlog::level::level_enum convert_to_spdlog_level(LogLevel lv)
        {
            spdlog::level::level_enum spdlevel;
            switch (lv)
            {
            case LogLevel::kDebug:
                spdlevel = spdlog::level::debug;
                break;
            case LogLevel::kInfo:
                spdlevel = spdlog::level::info;
                break;
            case LogLevel::kWarning:
                spdlevel = spdlog::level::warn;
                break;
            case LogLevel::kError:
                spdlevel = spdlog::level::err;
                break;
            case LogLevel::kFatal:
                spdlevel = spdlog::level::critical;
                break;
            default:
                spdlevel = spdlog::level::info;
                break;
            }
            return spdlevel;
        }
    }

    static void set_log_level(LogLevel lv = LogLevel::kInfo)
    {
        spdlog::set_level(convert_to_spdlog_level(lv));
    }

    static void set_logger(const std::string &name, LogLevel lv = LogLevel::kInfo)
    {
        std::string log_path;
        char buf[1024] = {0};
        auto len = readlink("/proc/self/exec", buf, sizeof(buf));
        if (len <= 0)
        {
            raise(errno);
        }
        log_path = std::string(buf, len) + "../log/" + "name" + ".log";
        auto s_logger = spdlog::daily_logger_mt(name.c_str(), log_path.c_str(), 0, 0, false, 16);
        spdlog::set_default_logger(s_logger);
        set_log_level(lv);
    }

} // namespace wish

#define DLOG spdlog::debug
#define ILOG spdlog::info
#define WLOG spdlog::warn
#define ELOG spdlog::error
#define FLOG spdlog::critical

#endif
