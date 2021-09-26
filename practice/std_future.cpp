#include <iostream>
#include <chrono>
#include <thread>
#include <future>
int main()
{

    {
        auto ft = std::async([] () {
            std::cout << "start async" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "end async" << std::endl;
            return "123";
        });
        std::cout << "start get" << std::endl;
        auto ret = ft.get();
        std::cout << "get " << ret << std::endl;
    }

    {
        auto ft = std::async([] () {
            std::cout << "start async" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "end async" << std::endl;
            return "456";
        });
        std::cout << "start check ready" << std::endl;
        while(ft.wait_for(std::chrono::seconds(1)) != std::future_status::ready)
        {
            std::cout << "not ready" << std::endl;
        }
        std::cout << "start get" << std::endl;
        auto ret = ft.get();
        std::cout << "get " << ret << std::endl;
    }

    {
        std::promise<std::string> prom;
        std::future ft = prom.get_future();

        std::thread([prom = std::move(prom)] () mutable {
            std::cout << "start async" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            prom.set_value("789");
            std::cout << "end async" << std::endl;
        }).detach();

        std::cout << "start get" << std::endl;
        auto ret = ft.get();
        std::cout << "get " << ret << std::endl;
    }

    {

        std::packaged_task<std::string(void)> pt([] () {
            std::cout << "start async" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "end async" << std::endl;
            return "000";
        });
        std::future ft = pt.get_future();

        std::thread(std::move(pt)).detach();

        std::cout << "start get" << std::endl;
        auto ret = ft.get();
        std::cout << "get " << ret << std::endl;
    }
}

