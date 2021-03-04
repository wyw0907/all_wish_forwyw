#include "core/threadpool/thread_pool.h"
#include "core/log.h"
#include <chrono>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>


using namespace std::chrono_literals;
int main(int argc, char **argv)
{
    std::string log_name("./log/");
    log_name += argv[0];
    log_name += ".log";
    auto logger = spdlog::daily_logger_mt(argv[0], log_name, 0, 0, false, 16);
    spdlog::set_default_logger(logger);
    spdlog::info("welcome");
    std::cout << "welcome" << std::endl;
    auto us = std::make_shared<int>();
    //daemon(0, 0);

    wish::safe_thread<int> st(us);

    // daemon 导致条件变量无法被唤醒

    st.insert([](auto&& _us) {
        std::cout << "do insert" << std::endl;
        spdlog::info("do insert");
    });

    std::string end = "";
    while (std::cin >> end)
    {
        if (tolower(end.at(0)) == 'q')
        {
            break;
        }
    }
     std::cout << "end" << std::endl;
}
