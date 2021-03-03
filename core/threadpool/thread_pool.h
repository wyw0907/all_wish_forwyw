#ifndef WISH_THREAD_POOL_H
#define WISH_THREAD_POOL_H
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std::chrono_literals;

namespace wish {

int current_thread_id();

template<typename INSTANCE>
class safe_thread
{
public:
    safe_thread(std::shared_ptr<INSTANCE> i) : instance(i)
    {
        worker = std::thread([this] {
            while(!this->stop)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lck(this->queue_mtx);
                    this->condition.wait(lck, [this] { return this->stop || !this->tasks.empty();} );
                    if (this->stop)
                    {
                        break;
                    }
                    else if (this->tasks.empty())
                    {
                        throw std::runtime_error("consume an empty task");
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });

    }

    virtual ~safe_thread()
    {
        stop = true;
        std::unique_lock<std::mutex> lck(queue_mtx);
        condition.notify_all();
        worker.join();
    }

    template<typename F, typename... Args>
    void insert(F f, Args... args)
    {
        auto task  = std::bind(std::forward<F>(f), instance, std::forward<Args>(args)...);
        std::unique_lock<std::mutex> lck(queue_mtx);
        if (stop)
        {
            throw std::runtime_error("insert into a stopped thread");
        }
        tasks.emplace(task);
        condition.notify_one();

    }

private:
    std::shared_ptr<INSTANCE>     instance;
    std::thread                   worker;
    std::queue<std::function<void()>> tasks;
    std::mutex                    queue_mtx;
    std::condition_variable       condition;
    bool                          stop = false;
};

}

#endif