#include "../../deps/rxcpp/include/rxcpp/rx.hpp"
#include "../../core/log.h"
#include <iostream>
namespace rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util; 
}

int main()
{
    /* 1
    创建一个可观察流
    */
    {
        //rx::observable::create<>用于创建一些可观察的对象, auto = rx::subscribe<int>
        auto observable = rx::observable<>::create<int>([] (auto s) {
                //发射事件
                s.on_next(1);
                s.on_next(2);

                //发射完成事件
                s.on_completed();
            }
        );
        //观察者订阅事件
        //第一个函数，形参为rx::observable<>::create<T>的模板类型，接收发射的信号on_next
        //第二个函数，形参为exception_ptr,用于接收被观察者抛出的异常
        //第三个函数，形参为void，用于接收结束信号，on_complated
        observable.subscribe(
            [] (int v) {
               LOG_DBG("1 OnNext %d", v);
            },
            [] (std::exception_ptr &ep) {
                try {
                    std::rethrow_exception(ep);
                }
                catch (std::exception &e)
                {
                    LOG_ERR("1 exception: ", e.what());
                }
            },
            [] () {
                LOG_INF("1 OnComplated");
            }
        );
    }

    /*2
    使用range返回一个可观察的流
    */
    {
        auto observable = rx::observable<>::range(1, 10);
        observable.subscribe(
            [] (int v) {
                LOG_DBG("2 OnNext: %d", v);
            },
            [] () {
                LOG_INF("2 OnComplated");
            }
        );
    }

    /*3
    使用filter过滤接收到的数据
    并使用map处理数据
    */
    {
        {
            auto observable = rx::observable<>::range(1, 10)
                .filter([] (int v) {
                    return v == 5;
                })
                .map([] (int v) {
                    return v * v;
                });
            
            observable.subscribe(
                [] (int v) {
                    LOG_DBG("3.1 OnNext: %d", v);
                },
                [] () {
                    LOG_INF("3.1 OnComplated");
                }
            );
        }
        {
            //map的另一种用法, 修改返回值类型
            auto observable = rx::observable<>::range(1, 10)
                .filter([] (int v) {
                    return v == 5;
                })
                .map([] (int v) -> std::string {
                    return std::to_string(v);
                });
            
            observable.subscribe(
                [] (const std::string & v) {
                    LOG_DBG("3.2 OnNext String: %s", v.c_str());
                },
                [] () {
                    LOG_INF("3.2 OnComplated");
                }
            );
        }
    }

    /* 4
    使用iterate迭代stl容器, 并返回一个可观察的流
    */
    {
        std::array<int, 5> a1{1,2,3,4,5};
        auto ob1 = rx::observable<>::iterate(a1);

        ob1.subscribe(
            [] (int v) {
                LOG_DBG("4.1 OnNext: %d", v);
            },
            [] () {
                LOG_INF("4.1 OnComplated");
            }
        );

        std::vector<std::string> a2;
        a2.emplace_back("test1");
        auto ob2 = rx::observable<>::iterate(a2);
        a2.emplace_back("test2");

        ob2.subscribe(
            [] (const std::string & v) {
                LOG_DBG("4.2 OnNext: %s", v.c_str());
            },
            [] () {
                LOG_INF("4.2 OnComplated");
            }
        );
        a2.emplace_back("test3");

        std::map<std::string, std::string> a3;
        a3.insert({"No.1", "test1"});
        auto ob3 = rx::observable<>::iterate(a3);
        a3.insert({"No.2", "test2"});
        ob3.subscribe(
            [] (const std::pair<std::string, std::string> & v) {
                LOG_DBG("4.3 OnNext: %s-%s", v.first.c_str(), v.second.c_str());
            },
            [] () {
                LOG_INF("4.3 OnComplated");
            }
        );
        a3.insert({"No.3", "test3"});
    }

    /*5
    使用concat或merge合并两个observable
    concat合并的结果是有序的，merge是无序的
    */
   {
        {
            auto ob1 = rx::observable<>::range(1, 10);
            auto ob2 = rx::observable<>::range(11, 20);
            auto sub_ob = ob1.concat(ob2);
            sub_ob.subscribe(
                [] (int v) {
                    LOG_DBG("5.1 OnNext: %d", v);
                },
                [] () {
                    LOG_INF("5.1 OnComplated");
                }
            );
        }
        {
            auto ob1 = rx::observable<>::range(1, 10);
            auto ob2 = rx::observable<>::range(11, 20);
            auto sub_ob = ob1.merge(ob2);
            sub_ob.subscribe(
                [] (int v) {
                    LOG_DBG("5.2 OnNext: %d", v);
                },
                [] () {
                    LOG_INF("5.2 OnComplated");
                }
            );
       }
   }

    /*6
    使用just创建发射单个事件的流
    */
    {
        class A
        {
        public:
            void set_num(int n)
            {
                _num = n;
            }
            const int & num() const
            {
                return _num;
            }
        private:
            int _num;
        };
        A a;
        a.set_num(15);
        rx::observable<>::just(a)
            .subscribe(
                [] (auto && v) {
                    LOG_DBG("6 OnNext: %d", v.num());
                },
                [] () {
                    LOG_INF("6 OnComplated");
                }
            );
    }

    /* 7 
    使用take限制发射次数，达到次数时发射一个结束事件
    */
    {   
        {
            //最简单的例子
            rx::observable<>::range(1, 10)
                .take(5) //发射前面5项
                .subscribe( [] (auto && v) {
                        LOG_DBG("7.1 OnNext: %d", v);
                    },
                    [] () {
                        LOG_INF("7.1 OnComplated");
                    }
                );
        }
        {
            // 总数少于take的场景
            rx::observable<>::range(1, 3)
                .take(5)
                .subscribe( [] (auto && v) {
                        LOG_DBG("7.2 OnNext: %d", v);
                    },
                    [] () {
                        LOG_INF("7.2 OnComplated");
                    }
                );
        }
        {
            // 出现异常时的场景
            rx::observable<>::create<int>([] (auto && s) {
                s.on_next(1);
                s.on_next(2);
                throw std::runtime_error("test error");
                s.on_next(3);
                s.on_completed();
            })
                .take(5)
                .subscribe( [] (auto && v) {
                        LOG_DBG("7.3 OnNext: %d", v);
                    },
                    [] (const std::exception_ptr & ep) {
                        try {
                            std::rethrow_exception(ep);
                        }
                        catch (const std::exception &e)
                        {
                            LOG_ERR("7.3 OnException : %s", e.what());
                        }
                    },
                    [] () {
                        LOG_INF("7.3 OnComplated");
                    }
                );
        }

    }

    /* 8
    取消订阅的方法
    */
    {
        auto subs = rx::composite_subscription();
        rx::observable<>::range(1, 10)
            .subscribe(
                subs,
                [&subs] (int v) {
                    LOG_DBG("8 OnNext: %d", v);
                    if (v == 3)
                    {
                        subs.unsubscribe();
                    }
                },
                [] () {
                    LOG_INF("8 OnComplated");
                }
            );

    }

    /* 9
    tap 一个从被观察者到观察者之间的一个角色，不同与filter和map, 不能修改返回值
    */
    {
        rx::observable<>::create<int>([] (auto && s) {
                s.on_next(1);
                s.on_next(2);
                throw std::runtime_error("test error");
                s.on_next(3);
                s.on_completed();
            })
            .tap(
                [] (int v) {
                    LOG_WRN("9 Tap -- OnNext : %d", v);
                },
                //即使这里注册了异常处理函数，该异常还是会被观察者捕获到
                [] (const std::exception_ptr & ep) {
                    try {
                        std::rethrow_exception(ep);
                    }
                    catch (const std::exception &e)
                    {
                        LOG_ERR("9 Tap -- OnException : %s", e.what());
                    }
                },
                [] () {
                    LOG_WRN("9 Tap -- OnComplete");
                }
            )
            .subscribe(
                [] (int v) {
                    LOG_DBG("9 OnNext: %d", v);
                },
                [] (const std::exception_ptr & ep) {
                    try {
                        std::rethrow_exception(ep);
                    }
                    catch (const std::exception &e)
                    {
                        LOG_ERR("9 OnException : %s", e.what());
                    }
                },
                [] () {
                    LOG_INF("9 OnComplated");
                } 
            );
    }


    /*
    rx::connectable_observable对象
    */
   {
       //auto ob = rx::connectable_observable<>::

   }

}







