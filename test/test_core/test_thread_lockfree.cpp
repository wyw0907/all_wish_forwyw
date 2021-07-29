
//#define LOCKFREE_NO_MEMPOOL
//#define LOCKFREE_COMPRESSION
#include "core/threadpool/thread_pool.h"
#include <iostream>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <memory>
#define CASE_SIZE 1000000
static std::string test_case = "wuyunwei ai dongyue @ 20170715";
struct MemTest
{
    char buf[4096];
    MemTest()
    {
        memset(buf, 1, 4096);
    }
    MemTest& operator=(const MemTest& other)
    {
        memmove(buf, other.buf, 4096);
        return *this;
    }
    MemTest& operator=(MemTest&& other)
    {
        memmove(buf, other.buf, 4096);
        return *this;
    }
    MemTest(const MemTest& other)
    {
        memmove(buf, other.buf, 4096);
    }
    MemTest(MemTest&& other)
    {
        memmove(buf, other.buf, 4096);
    }
};
int main()
{
    auto lockfree_tp = wish::lockfree_queue<std::string, 0>();
    lockfree_tp.emplace(test_case);
    std::string x;
    while(lockfree_tp.pop_front(x))
    {
        std::cout << "consume:" << x << " left:" << lockfree_tp.size() << std::endl;
    }

    lockfree_tp.emplace(test_case);
    lockfree_tp.emplace(test_case);
    if (lockfree_tp.pop_front(x))
    {
        std::cout << "consume:" << x << " left:" << lockfree_tp.size() << std::endl;
    }
    lockfree_tp.emplace(test_case);
    while(lockfree_tp.pop_front(x))
    {
        std::cout << "consume:" << x << " left:" << lockfree_tp.size() << std::endl;
    }

    auto lockfree_tp2 = wish::lockfree_queue<MemTest, 100>();
    lockfree_tp2.emplace(MemTest());
    MemTest y;
    while(lockfree_tp2.pop_front(y))
    {
        //std::cout << "consume:" << y.buf << " left:" << lockfree_tp2.size() << std::endl;
    }

     
    {
            auto lockfree_tp = wish::lockfree_queue<std::string>();
        auto clk = std::chrono::steady_clock::now();
        for (size_t i = 0; i < CASE_SIZE; ++i)
        {
            lockfree_tp.emplace(test_case);
        }
        while(lockfree_tp.pop_front(test_case))
        {
            //std::cout << "consume:" << x << " left:" << lockfree_tp.size() << std::endl;
        }
        auto clk2 = std::chrono::steady_clock::now();
        std::cout << "lockfree cost " << std::chrono::duration_cast<std::chrono::microseconds>(clk2 - clk).count() << "us" << std::endl;
        // ~230ms
    }
   
    {
        auto que = std::queue<int>();
        auto clk = std::chrono::steady_clock::now();
        for (size_t i = 0; i < CASE_SIZE; ++i)
        {
            que.emplace(i);
        }
        while(!que.empty())
        {
            int x = que.front();
            (void) x;
            que.pop();
            //std::cout << "consume:" << x << " left:" << lockfree_tp.size() << std::endl;
        }
        auto clk2 = std::chrono::steady_clock::now();
        std::cout << "queue cost " << std::chrono::duration_cast<std::chrono::microseconds>(clk2 - clk).count() << "us" << std::endl;
        // ~4ms
    }

    {
        auto que = std::queue<int>();
        std::mutex mtx;
        auto clk = std::chrono::steady_clock::now();
        for (size_t i = 0; i < CASE_SIZE; ++i)
        {
            mtx.lock();
            que.emplace(i);
            mtx.unlock();
        }
        while(true)
        {
            mtx.lock();
            if (que.empty())
            {
                break;
            }
            int x = que.front();
            (void) x;
            que.pop();
            mtx.unlock();
            //std::cout << "consume:" << x << " left:" << lockfree_tp.size() << std::endl;
        }
        auto clk2 = std::chrono::steady_clock::now();
        std::cout << "queue mtx cost " << std::chrono::duration_cast<std::chrono::microseconds>(clk2 - clk).count() << "us" << std::endl;
        // ~50ms
    }
    {
        auto tp = std::queue<MemTest>();
        std::mutex mtx;
        std::condition_variable cond;
        auto clk = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        // 8核， 8写一读
        auto r = std::thread([&tp, &clk, &mtx, &cond] () {
            size_t cnt = 0;
            while(true)
            {
                //mtx.lock();
                std::unique_lock<std::mutex> lck(mtx);
                cond.wait_for(lck, std::chrono::milliseconds(1000));
                while(!tp.empty())
                {
                    MemTest x = tp.front();
                    (void) x;
                    tp.pop();
                    ++cnt;
                }
                
                if (cnt == 8 * CASE_SIZE)
                {
                    break;
                }
            }
            auto clk2 = std::chrono::steady_clock::now();
            std::cout << "queue 8w1r cost " << std::chrono::duration_cast<std::chrono::microseconds>(clk2 - clk).count() << "us" << std::endl;
            // ~10s
        });
        int i = 0;
        while(i ++ < 8)
        {
            std::thread([&tp, &clk, &mtx, &cond] () {
                std::this_thread::sleep_until(clk);
                for (size_t i = 0; i < CASE_SIZE; ++i)
                {
                    mtx.lock();
                    tp.emplace(MemTest());
                    cond.notify_one();
                    mtx.unlock();
                }
            }).detach();
        }
        r.join();
    }
    {
        wish::event ev;
        auto tp = wish::lockfree_queue<MemTest, 1024>();
        auto clk = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        // 8核， 8写一读
        auto r = std::thread([&tp, &clk, &ev] () {
            size_t cnt = 0;
            while(true)
            {
                if (ev.wait(std::chrono::milliseconds(1000)))
                {
                    MemTest x;
                    while(tp.pop_front(x))
                    {
                        (void ) x;
                        ++cnt;
                    }
                }
                if (cnt == 8 * CASE_SIZE)
                {
                    break;
                }
                
            }
            auto clk2 = std::chrono::steady_clock::now();
            std::cout << "lockfree 8w1r cost " << std::chrono::duration_cast<std::chrono::microseconds>(clk2 - clk).count() << "us" << std::endl;
            //~6s
        });

        int i = 0;
        while(i ++ < 8)
        {
            std::thread([&tp, &clk, i, &ev] () {
                std::this_thread::sleep_until(clk);
                for (size_t i = 0; i < CASE_SIZE; ++i)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                    tp.emplace(MemTest());
                    ev.set();
                }
            }).detach();
        }
        r.join();
    }
    {
        
        auto tp = moodycamel::BlockingConcurrentQueue<MemTest>();
        auto clk = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        // 8核， 8写一读
        auto r = std::thread([&tp, &clk] () {
            size_t cnt = 0;
            moodycamel::ConsumerToken ctok(tp);
            while(true)
            {
                MemTest x;
                while(tp.wait_dequeue_timed(ctok, x, std::chrono::seconds(1)))
                {
                    (void )x;
                    ++cnt;
                    if (cnt == 8 * CASE_SIZE)
                    {
                        break;
                    }
                }
                if (cnt == 8 * CASE_SIZE)
                {
                    break;
                }
            }
            auto clk2 = std::chrono::steady_clock::now();
            std::cout << "BlockingConcurrentQueue 8w1r cost " << std::chrono::duration_cast<std::chrono::microseconds>(clk2 - clk).count() << "us" << std::endl;
            //
        });

        int i = 0;
        while(i ++ < 8)
        {
            std::thread([&tp, &clk, i] () {
                moodycamel::ProducerToken ptok(tp);
                std::this_thread::sleep_until(clk);
                
                for (size_t i = 0; i < CASE_SIZE; ++i)
                {
                    while(!tp.enqueue(ptok, MemTest()));
                }
            }).detach();
        }
        r.join();
    }

}