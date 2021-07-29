#ifndef WISH_THREAD_POOL_H
#define WISH_THREAD_POOL_H
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <atomic>
#include "event.h"
#include "lockfree/lockfree_queue.h"
#include "concurrentqueue/blockingconcurrentqueue.h"
using namespace std::chrono_literals;

namespace wish {

extern uint32_t current_thread_id();
extern std::string current_thread_name();
extern void set_thread_name(const std::string& name);

template <template <typename...> class U, typename T>
struct is_template_instant_of : std::false_type
{
};

template <template <typename...> class U, typename... Args>
struct is_template_instant_of<U, U<Args...>> : std::true_type
{
};

/*
    线程池模板
*/

template <class Queue>
class thread_pool_base
{
public:
    using Item = typename Queue::value_type;
    thread_pool_base(
        size_t pool_size = 0, 
        const std::string& name = "", 
        std::chrono::milliseconds duration = std::chrono::milliseconds(1000))
    {
        if (pool_size == 0)
        {
            pool_size = std::thread::hardware_concurrency();
        }
        for (size_t i = 0; i < pool_size; ++i)
        {
            m_workers.emplace_back(std::thread([this, i, name, duration] {
                std::string thread_name = name.empty() ? "thread_pool_base" : name;
                set_thread_name(thread_name + "_" + std::to_string(i));
                try 
                {
                    while(!this->m_stop)
                    {
                        Item task;
                
                        if(m_tasks.block_pop_item(task, duration))
                        {
                            if (!this->m_stop)
                            {
                                try {
                                    task();
                                }
                                catch(const std::exception& e)
                                {
                                    std::cout << current_thread_name() << " task exception: " << e.what() << std::endl;
                                    throw;
                                }
                            }
                        }
                    }
                }
                catch(const std::exception& e)
                {
                    std::cout << current_thread_name() << " exception: " << e.what() << std::endl;
                                                        throw;

                }
                std::cout << current_thread_name() << " exit. " << std::endl;
            }));
        }
    }

    virtual ~thread_pool_base()
    {
        if (!m_stop)
        {
            m_stop = true;
            m_tasks.exit_block();
        }
        for(auto& worker : m_workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    void wait()
    {
        for(auto& worker : m_workers)
        {
            worker.join();
        }
    }

    void stop()
    {
        if (!m_stop)
        {
             std::cout << "stop" << std::endl;

            m_stop = true;
            m_tasks.exit_block();
        }
    }

    template<typename F, typename... Args>
    bool insert(F&& f, Args&&... args)
    {
        auto task  = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        if (this->m_stop)
        {
            throw std::runtime_error("insert into a stopped thread");
        }
        return m_tasks.push_item(std::move(task));
    }

    size_t size() const
    {
        return m_tasks.size();
    }

    size_t nums() const
    {
        return m_workers.size();
    }

    bool empty() const
    {
        return size() == 0;
    }

    bool is_stop() const
    {
        return m_stop.load();
    }

protected:
    std::vector<std::thread>        m_workers;
    Queue                           m_tasks;
    std::atomic<bool>               m_stop{false};

};

template <class Queue = std::queue<std::function<void()>>>
class queue_wrap : protected Queue
{
public:
    using value_type = typename Queue::value_type;
    size_t size() const
    {
        return Queue::size();
    }

    void exit_block()
    {
        std::unique_lock<std::mutex> lck(this->m_queue_mtx);
        this->m_condition.notify_all();
        lck.unlock();
    }

    using Item = typename Queue::value_type;
    template<class Duration>
    bool block_pop_item(Item& task, const Duration& duration)
    {
        std::unique_lock<std::mutex> lck(this->m_queue_mtx);
        this->m_condition.wait_for(lck, duration, [this] () {return !this->empty();});
        if (this->empty())
        {
            return false;
        }
        task = std::move(this->front());
        this->pop();
        return true;
    }

    template<class T>
    bool push_item(T&& t)
    {
        std::unique_lock<std::mutex> lck(this->m_queue_mtx);
        this->emplace(std::forward<T>(t));
        this->m_condition.notify_one();
        return true;
    }
protected:
    std::mutex                      m_queue_mtx;
    std::condition_variable         m_condition;
};

/*
    基于stl queue实现的线程池
*/
using thread_pool = thread_pool_base<queue_wrap<>>;

/*
    优先队列线程池模板
*/
template<class Queue>
class priority_thread_pool_base : public thread_pool_base<Queue>
{
public:
    using Item = typename Queue::value_type;
    using thread_pool_base<Queue>::thread_pool_base;

    template<typename F, typename... Args>
    bool zinsert(F&& f, const typename Item::score_type& score, Args&&... args)
    {
        auto task  = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        if (this->m_stop)
        {
            throw std::runtime_error("zinsert into a stopped thread");
        }
        return this->m_tasks.push_item(Item(std::move(task), score));
    }

    template<typename F, typename... Args>
    bool insert(F&& f, Args&&... args)
    {
        typename Item::score_type score;
        return zinsert(std::forward<F>(f), std::move(score), std::forward<Args>(args)...);
    }
};

template <class Item, class Queue = std::priority_queue<Item>>
class priority_queue_wrap : public queue_wrap<Queue>
{
public:
    using super = queue_wrap<Queue>;
    using value_type = typename Queue::value_type;

    template<class Duration>
    bool block_pop_item(Item& task, const Duration& duration)
    {
        std::unique_lock<std::mutex> lck(this->m_queue_mtx);
        this->m_condition.wait_for(lck, duration, [this] () {return !this->empty();});
        if (this->empty())
        {
            return false;
        }
        task = std::move(this->top());
        this->pop();
        return true;
    }
};

/*
    优先队列元素模板
*/

template<class T, class Comp = std::less<T>>
class priority_queue_item
{
public:
    using score_type = T;
    priority_queue_item(std::function<void()>&& func, const score_type& score) 
        : m_func(std::move(func)), m_score(score)
    {
    }
    priority_queue_item()
    {
    }
    priority_queue_item(const priority_queue_item& other) = default;
    priority_queue_item& operator=(const priority_queue_item& other) = default;
    priority_queue_item(priority_queue_item&& other) = default;
    priority_queue_item& operator=(priority_queue_item&& other) = default;
    bool operator<(const priority_queue_item& other) const
    {
        return Comp()(m_score, other.m_score);
    }
    void operator()()
    {
        m_func();
    }
    const score_type& score() const
    {
        return m_score;
    }

private:
    std::function<void()>   m_func;
    score_type     m_score;
};

/*
    基于stl priority_queue实现的优先队列线程池
    特点：由于std::priority_queue基于二叉堆实现，相同score的会出现不稳定的排序，导致失去队列先进先出的特性
    可以特化priority_queue_item实现其他类型的score排序
*/

using priority_thread_pool = priority_thread_pool_base<priority_queue_wrap<priority_queue_item<int32_t>>>;

template<class T>
class comp_more : public std::binary_function<T, T, bool>
{
public:
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs > rhs;
    }
};

template<class Item>
class stable_priority_queue
{
public:
    using value_type = Item;
    //红黑树达到最深节点复杂度log N, 所以选择从头部pop, 这样的话排序需要从大到小排
    value_type& top()
    {
        if(m_sorted_queue.empty())
        {
            throw std::logic_error("empty queue");
        }
        return m_sorted_queue.begin()->second;
    }
    const value_type& top() const
    {
        if(m_sorted_queue.empty())
        {
            throw std::logic_error("empty queue");
        }
        return m_sorted_queue.cbegin()->second;
    }

    void pop()
    {
        auto iter = m_sorted_queue.begin();
        if (iter != m_sorted_queue.end())
        {
            m_sorted_queue.erase(iter);
        }
    }

    size_t size() const
    {
        return m_sorted_queue.size();
    }

    bool empty() const
    {
        return size() == 0;
    }

    void emplace(Item&& item)
    {
        m_sorted_queue.emplace(std::pair{item.score(), std::move(item)});
    }

    void emplace(const Item& item)
    {
        m_sorted_queue.emplace(std::pair{item.score(), item});
    }

private:
    std::multimap<typename Item::score_type, Item, comp_more<typename Item::score_type>> m_sorted_queue;
};

/*
    基于stl multimap实现的优先队列线程池
    特点：与std::priority_queue相比，相同score的元素会呈现先进先出的特性，缺点是如果key较多，红黑树相比二叉堆性能有所不足
    可以特化priority_queue_item实现其他类型的score排序
*/

using stable_priority_thread_pool = priority_thread_pool_base<priority_queue_wrap<priority_queue_item<int32_t>, stable_priority_queue<priority_queue_item<int32_t>>>>;

template<class Item = std::function<void()>, size_t PoolSize = 1024, class Queue = wish::lockfree_queue<Item, PoolSize>>
class lockfree_queue_wrap : public queue_wrap<Queue>
{
public:
    using value_type = typename Queue::value_type;
    template<class Duration>
    bool block_pop_item(Item& task, const Duration& duration)
    {
        if(this->empty())
        {
            std::unique_lock<std::mutex> lck(this->m_queue_mtx);
            this->m_condition.wait_for(lck, duration, [this] () {return !this->empty();});
            lck.unlock();
        }
        return this->pop_front(task);
    }
    template<class T>
    bool push_item(T&& t)
    {
        bool ret = this->emplace(std::forward<T>(t));
        if(ret)
        {
            std::unique_lock<std::mutex> lck(this->m_queue_mtx);
            this->m_condition.notify_one();
        }
        return ret;
    }
};

/*
    基于lockfree_queue实现的无锁队列线程池
    特点：与std::priority_queue相比，相同score的元素会呈现先进先出的特性，缺点是如果key较多，红黑树相比二叉堆性能有所不足
    可以特化priority_queue_item实现其他类型的score排序
*/

using lockfree_thread_pool = thread_pool_base<lockfree_queue_wrap<std::function<void()>, 1024>>;

/*
    数据安全线程
*/
template<class Instance, class ThreadPool, 
    typename = typename std::enable_if_t<is_template_instant_of<thread_pool_base, ThreadPool>::value 
                                    || is_template_instant_of<priority_thread_pool_base, ThreadPool>::value>>
class safe_thread : public ThreadPool
{
public:
    safe_thread(std::shared_ptr<Instance> ins, const std::string& name = "") 
        : m_instance(ins), 
          ThreadPool(1, name)
    {
    }

    template<typename F, typename... Args>
    void insert(F&& f, Args&&... args)
    {
        ThreadPool::insert(std::forward<F>(f), m_instance, std::forward<Args>(args)...);
    }

    template<typename F, typename Arg0, typename... Args>
    void zinsert(F&& f, Arg0&& arg0, Args&&... args)
    {
        if constexpr (is_template_instant_of<priority_thread_pool_base, ThreadPool>::value)
        {
            ThreadPool::zinsert(std::forward<F>(f), std::forward<Arg0>(arg0), m_instance, std::forward<Args>(args)...);
        }
        else
        {
            static_assert("zinsert is invalid");
        }
    }

private:
    std::shared_ptr<Instance>     m_instance;
};

}

#endif