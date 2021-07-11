#include "core/threadpool/thread_pool.h"
#include <iostream>
#include <chrono>
#include <cstdio>
#include <cstring>
const char box[] = "1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,./!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:ZXCVBNM<>?";

void reverse_string()
{
    std::string str;
    size_t i = 0;
    while(i++ < 1000)
    {
        str.append(box[i % strlen(box)], 1);
        std::reverse(std::begin(str), std::end(str));
    }
}


int main(int argc, char **argv)
{

    //auto tp = wish::thread_pool(8, "thread_pool");
    // no task : thread size 8, cost ~180ms, size 1, cost ~70ms
    // do reverse_string:  thread size 8, cost ~180ms, size 1, cost ~60ms

    //auto tp = wish::priority_thread_pool(1, "priority_thread_pool");
    // thread size 8, cost ~200ms, size 1, cost ~120ms
    // do reverse_string:  thread size 8, cost ~34'000ms, size 1, cost ~130'000 ms

    auto tp = wish::stable_priority_thread_pool(1, "priority_thread_pool");
    // thread size 8, cost ~200ms, size 1, cost ~120ms
    // do reverse_string:  thread size 8, cost ~34'000ms, size 1, cost ~130'000 ms
    if (argv[1] && strcmp(argv[1], "--test-priority") == 0)
    {
        size_t size = 0;
        while(size ++ < 100)
        {
        //    std::thread([&tp, size] () {
                tp.zinsert([] (size_t size2) {
                    printf("[%s] %lu\n", wish::current_thread_name().c_str(), size2);
                    std::this_thread::sleep_for(10ms);
                }, size / 10, size);
        //    }).detach();
        }
        tp.wait();
        return 0;
    }
    int size = 0;
    int sum = 10000;
    std::atomic<int> count {0};
    auto start = std::chrono::steady_clock::now();
    while (size ++ < sum)
    {
        tp.insert([&start, &count, &sum] (size_t size2) {
            printf("[%s] %lu %d\n", wish::current_thread_name().c_str(), size2, count.load());
            //reverse_string();
            ++count;
            if (count == sum)
            {
                auto end = std::chrono::steady_clock::now();
                printf("cost %lf ms\n", double(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000);
            }
        }, size);
    }
    
    tp.wait();

   
}