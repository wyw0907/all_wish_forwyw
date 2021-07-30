#include "core/threadpool/thread_pool.h"
#include <iostream>
#include <chrono>
#include <cstdio>
#include <cstring>

int main(int argc, char **argv)
{
    auto tp = wish::priority_thread_pool(8, "priority_thread_pool");
    // 1w8r , task 1000'000 cost 26318.194 ms
    // 1w1r , task 1000'000 cost 18241.205 ms
    //auto tp = wish::stable_priority_thread_pool(8, "stable_priority_thread_pool");
    // 1w8r , task 1000'000 cost 26196.784 ms
    // 1w1r , task 1000'000 cost 19287.723 ms
    //auto tp = wish::static_stable_priority_thread_pool<10>(8, "static_stable_priority_thread_pool");
    // 1w8r , task 1000'000 cost 27292.491 ms 10score
    // 1w1r , task 1000'000 cost 19702.628 ms 10score
    
    size_t size = 0;
    std::atomic<int> count {0};
    auto start = std::chrono::steady_clock::now();

    while(size < 1000000)
    {
        //std::thread([&tp, size] () {
            tp.zinsert([&tp, &start, &count] (size_t size2) {
                //std::this_thread::sleep_for(1ms);
                ++count;
                printf("[%s] %lu %d\n", wish::current_thread_name().c_str(), size2, count.load());
                if (count == 1000000)
                {
                    auto end = std::chrono::steady_clock::now();
                    printf("cost %.3lf ms\n", double(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000);
                    tp.stop();
                }

            }, size / 10, size);
        //}).detach();
        size++;
    }
    tp.wait();
    return 0;

}