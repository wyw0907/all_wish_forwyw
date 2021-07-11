#include "core/threadpool/thread_pool.h"
#include "core/log/log.h"
#include <iostream>
// a non-thread-safe class                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
class UnSafe
{
public:
    void add(int x, int y)
    {
        while(y -- > 0)
        {
            counts += x;
        }
    }

    void print()
    {
        LOG_DBG("counts: %d", counts);
    }

    int counts = 0;
};


using namespace std::chrono_literals;
int main()
{
    int cnts = 10000;
    const int interval = 5;
    const int times = 10;
    auto us = std::make_shared<UnSafe>();
    while(cnts -- > 0)
    {
        std::thread([&us] () {
            us->add(interval, times);
        }).detach();
    }
    std::this_thread::sleep_for(3s);
    us->print();


    cnts = 10000;
    us->counts = 0;
    wish::safe_thread<UnSafe, wish::thread_pool> st(us);
    while(cnts -- > 0)
    {
        std::thread([&st, interval, times] () {
            st.insert([] (std::shared_ptr<UnSafe> _us, int _interval, int _times) {
                _us->add(_interval, _times);
            }, interval, times);
        }).detach();
    }
    std::this_thread::sleep_for(3s);
    us->print();


    cnts = 10000;
    us->counts = 0;
    wish::safe_thread<UnSafe, wish::stable_priority_thread_pool> st2(us);
    while(cnts -- > 0)
    {
        std::thread([&st2, cnts, interval, times] () {
            st2.zinsert([] (std::shared_ptr<UnSafe> _us, int _interval, int _times) {
                _us->add(_interval, _times);
            }, cnts, interval, times);
        }).detach();
    }
    std::this_thread::sleep_for(3s);
    us->print();


    std::string end = "";
    while(std::cin >> end)
    {
        if (tolower(end.at(0)) == 'q')
        {
            break;
        }

    }
}

