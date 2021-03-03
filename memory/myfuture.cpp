#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <unistd.h>

template<typename T>
class myFuture
{
public:
    myFuture(std::future<T> && ft)
    {
        m_thread = std::thread([this] (std::future<T> && future) {
            for (;;)
            {
                future.wait();
                std::function<void(T)> task;
                {
                    std::lock_guard<std::mutex> lck(m_mtx);
                
                    if (m_thentasks.empty())
                    {
                        std::cout << "no then tasks" << std::endl;
                        break;
                    }
                    task = std::move(m_thentasks.front());
                    m_thentasks.pop();
                }
                task(std::move(future.get()));
                std::function<T()> whentask;
                {
                    std::lock_guard<std::mutex> lck(m_mtx2);
                    if (m_whentasks.empty())
                    {
                        break;
                    }
                    whentask = std::move(m_whentasks.front());
                    m_whentasks.pop();
                }
                auto fft = std::async(whentask);
                future = std::move(fft);
            }
        }, std::move(ft));
    }    


    ~myFuture()
    {
        m_thread.join();
    }

    template<typename F>
    myFuture && Then(F f)
    {
        auto task = std::bind(std::forward<F>(f), std::placeholders::_1);
        std::lock_guard<std::mutex> lck(m_mtx);
        m_thentasks.emplace(task);
        return std::move(*this);
    }
    
    /*template<typename F, typename ...Args>
    myFuture && Then(F f, Args... arg)
    {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(arg)...);
        std::lock_guard<std::mutex> lck(m_mtx);
        m_thentasks.emplace(task);
        return std::move(*this);
    }*/
    template<typename F, typename ...Args>
    myFuture && When(F f, Args... arg)
    {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(arg)...);
        std::lock_guard<std::mutex> lck(m_mtx2);
        m_whentasks.emplace(task);
        return std::move(*this);
    }


private:
    std::queue<std::function<void(T)>> m_thentasks;
    std::queue<std::function<T()>>    m_whentasks;
    std::mutex                  m_mtxfuture;
    std::condition_variable      m_condfuture;
    std::mutex      m_mtx;
    std::mutex      m_mtx2;
    int xxx;
    std::thread     m_thread;
};

template<typename F, typename ...Args>
auto Execute(F && f, Args&&... arg) -> myFuture<typename std::result_of<F(Args...)>::type> && 
{
    auto ft = std::async(std::forward<F>(f), std::forward<Args>(arg)...);
    return std::move(myFuture<typename std::result_of<F(Args...)>::type>(std::move(ft)));
}

int main()
{
/*    Execute([] () -> int{
        std::cout << "execute" << std::endl;
        sleep(1);
        return 1;
    }).Then([] (int x) {
        std::cout << "then " << x << std::endl;
    }).When([] () -> int {
        std::cout << "when 1" << std::endl;
        sleep(2);
        return 2;
    }).Then([] (int x) {
        std::cout << "then " << x << std::endl;
    });
*/
   auto && ft = Execute([] () -> int{
       std::cout << "execute" << std::endl;
       sleep(1);
        return 1;
   }).Then([] (int x) {
        std::cout << "then " << x << std::endl;
    });
   sleep(10);
}



