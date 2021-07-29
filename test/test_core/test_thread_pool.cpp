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

struct MemTest
{
    char buf[4096];
    MemTest()
    {
        memset(buf, 1, 4096);
        //std::cout << "new" << std::endl;
    }
    ~MemTest()
    {
        //std::cout << "delete" << std::endl;

    }
    MemTest& operator=(const MemTest& other)
    {
        memmove(buf, other.buf, 4096);
        //std::cout << "copy=" << std::endl;

        return *this;
    }
    MemTest& operator=(MemTest&& other)
    {
        memmove(buf, other.buf, 4096);
        //std::cout << "move=" << std::endl;

        return *this;
    }
    MemTest(const MemTest& other)
    {
        memmove(buf, other.buf, 4096);
        //std::cout << "copy" << std::endl;

    }
    MemTest(MemTest&& other)
    {
        memmove(buf, other.buf, 4096);
        //std::cout << "move" << std::endl;

    }
};

int main(int argc, char **argv)
{

    //auto tp = wish::thread_pool(8, "thread_pool");
    // no task : thread size 8, cost ~240ms, size 1, cost ~180ms
    // do reverse_string:  thread size 8, cost ~180ms, size 1, cost ~60ms
    // thread8 task 1000'000 cost 26032.683 ms
    
    //auto tp = wish::priority_thread_pool(8, "priority_thread_pool");
    // thread size 8, cost ~400ms, size 1, cost ~300ms
    // do reverse_string:  thread size 8, cost ~34'000ms, size 1, cost ~130'000 ms

    //auto tp = wish::stable_priority_thread_pool(8, "priority_thread_pool");
    // thread size 8, cost ~270ms, size 1, cost ~200ms
    // do reverse_string:  thread size 8, cost ~34'000ms, size 1, cost ~130'000 ms
    
    auto tp = wish::lockfree_thread_pool(8, "lockfree_thread_pool");
    // thread size 8, cost ~220ms, size 1, cost ~160ms
    // thread8 task 1000'000 cost 29998.103 ms

    if (argv[1] && strcmp(argv[1], "--test-priority") == 0)
    {
        size_t size = 0;
        while(size ++ < 100)
        {
        /*    std::thread([&tp, size] () {
                tp.zinsert([] (size_t size2) {
                    printf("[%s] %lu\n", wish::current_thread_name().c_str(), size2);
                    std::this_thread::sleep_for(10ms);
                }, size / 10, size);
                }).detach();
        */
        }
        tp.wait();
        return 0;
    }
    int size = 0;
    int sum = 1000000;
    MemTest test;
    std::atomic<int> count {0};
    auto start = std::chrono::steady_clock::now();
    while (size < sum/2)
    {

        tp.insert([&start, &count, &sum, &tp, test=std::move(test)] (size_t size2) {

            //reverse_string();
            ++count;
            printf("[%s] function1 %lu %d\n", wish::current_thread_name().c_str(), size2, count.load());

            if (count == sum)
            {
                auto end = std::chrono::steady_clock::now();
                printf("cost %.3lf ms\n", double(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000);
                tp.stop();
            }

        }, size);
        size ++;
    }

    while (size < sum)
    {

        tp.insert([&start, &count, &sum, &tp, test=std::move(test)] (size_t size2) {

            //reverse_string();
            ++count;
            printf("[%s] function2 %lu %d\n", wish::current_thread_name().c_str(), size2, count.load());

            if (count == sum)
            {
                auto end = std::chrono::steady_clock::now();
                printf("cost %.3lf ms\n", double(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000);
                tp.stop();
            }

        }, size);
        size ++;
    }
    tp.wait();


}