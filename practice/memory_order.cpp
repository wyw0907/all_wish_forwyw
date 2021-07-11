
#include <atomic>
#include <thread>
#include <iostream>
#include <cassert>
std::atomic<bool> a{false};
std::atomic<bool> b{false};
int main()
{
    std::thread([] () {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        a.store(true, std::memory_order_relaxed);
        b.store(true, std::memory_order_release);
    }).detach();

    auto t2 = std::thread([] () {
        while(!b.load(std::memory_order_acquire));
        assert(a.load(std::memory_order_relaxed));
    });

    t2.join();
}