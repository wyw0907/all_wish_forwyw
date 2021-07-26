#ifndef WISH_LOCKFREE_QUEUE
#define WISH_LOCKFREE_QUEUE
#include <atomic>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "tagged_ptr.h"
#include "freelist.h"
namespace wish
{

template<class T, size_t PoolSize = 1024, bool Compression = false>
class lockfree_queue
{
public:
    using value_type = T;
    struct node
    {
        template <class U>
        node(tagged_ptr<node, Compression> ptr, U&& u) : next(ptr), data(std::forward<U>(u))
        {
        }
        node(tagged_ptr<node, Compression> ptr): next(ptr)
        {
        }

        std::atomic<tagged_ptr<node, Compression>> next;
        value_type                    data; 
    };
    using tagged_node_ptr = tagged_ptr<node, Compression>;
    using atomic_tagged_node_ptr = std::atomic<tagged_node_ptr>;
public:
    lockfree_queue() : 
        m_pool(), 
        m_head(tagged_node_ptr(nullptr, 0)), 
        m_tail(tagged_node_ptr(nullptr, 0)), 
        m_size(0)
    {
        auto null_node = m_pool.construct(tagged_node_ptr(nullptr, 0));
        tagged_node_ptr null_tag(null_node, 0);
        m_head.store(null_tag);
        m_tail.store(null_tag);
    }

    bool pop_front(value_type & ret)
    {
        for (;;)
        {
            auto head = m_head.load(std::memory_order_acquire);
            auto tail = m_tail.load(std::memory_order_acquire);
            auto next = head->next.load(std::memory_order_acquire);
            //std::cout << "11111 ";
            //std::cout << std::hex << head.get_ptr() << " "<< head.get_tag() << " - ";
            //std::cout << std::hex << next.get_ptr() << " " << next.get_tag() << " - ";
            //std::cout << std::hex << tail.get_ptr() << " "<< tail.get_tag() << " - ";

            //std::cout  << std::endl;
            //sleep(1);
            if (head == tail)
            {
                // 空链表 
                if (next.get_ptr() == nullptr)
                {
                    return false;
                }
                //但是存在next
                //其他线程正在操作，在head后面添加了next节点,帮它完成下一步
                tagged_node_ptr new_tail(next.get_ptr(), tail.get_next_tag());
                m_tail.compare_exchange_strong(tail, new_tail, std::memory_order_acq_rel);
            }
            else
            {
                // 非空链表，但是next为空，需要等待其他正在消费的节点完成
                if (next.get_ptr() == nullptr)
                {
                    continue;
                }

                // 首节点不存数据，恒为空，每次消费次节点，然后把次节点挂在首节点位置
                value_type *data = &next->data;
                tagged_node_ptr new_head(next.get_ptr(), head.get_next_tag());
                if (m_head.compare_exchange_weak(head, new_head, std::memory_order_acq_rel))
                {
                    ret = std::move(*data);
                    m_pool.destruct(head.get_ptr());
                    m_size.fetch_sub(1ul, std::memory_order_relaxed);
                    return true;
                }
                // 没挂成功，重新来过
            }
        }
    }

    bool push_back(node* _node)
    {
        for(;;)
        {
            //auto head = m_head.load(std::memory_order_acquire);
            auto tail = m_tail.load(std::memory_order_acquire);
            auto next = tail->next.load(std::memory_order_acquire);
            //            std::cout << "22222 ";
            //std::cout << std::hex << head.get_ptr() << " "<< head.get_tag() << " - ";
            //std::cout << std::hex << tail.get_ptr() << " "<< tail.get_tag() << " - ";
            //std::cout << std::hex << next.get_ptr() << " " << next.get_tag() << " - ";

            //std::cout << std::endl;
            //sleep(1);
            if (next.get_ptr() == nullptr)
            {
                tagged_node_ptr new_tail_next(_node, next.get_next_tag());
                if (tail->next.compare_exchange_weak(next, new_tail_next, std::memory_order_acq_rel))
                {
                    tagged_node_ptr new_tail(_node, tail.get_next_tag());
                    m_tail.compare_exchange_strong(tail, new_tail, std::memory_order_acq_rel);
                    // 这里size有概率溢出，考虑到如果使用自旋保证size会影响性能，所以依然这么处理
                    m_size.fetch_add(1ul, std::memory_order_relaxed);
                    return true;
                }
            }
            else
            {
                //其他线程正在操作，在tail后面添加了next节点,帮它完成下一步
                tagged_node_ptr new_tail(next.get_ptr(), tail.get_next_tag());
                m_tail.compare_exchange_strong(tail, new_tail, std::memory_order_acq_rel);
            }
        }
    }
    template <class U, typename = typename std::enable_if_t<std::is_same_v<value_type, std::decay_t<U>>>>
    bool emplace(U&& item)
    {
        auto _node = m_pool.construct(tagged_node_ptr(nullptr, 0), std::forward<U>(item));
        if (_node == nullptr)
        {
            return false;
        }
        return push_back(_node);
    }

    size_t size() const
    {
        return m_size.load(std::memory_order_relaxed);
    }

    bool empty() const
    {
        return m_head.load(std::memory_order_relaxed) == m_tail.load(std::memory_order_relaxed);
    }

private:
    atomic_tagged_node_ptr m_head;
    atomic_tagged_node_ptr m_tail;
    std::atomic<size_t>    m_size;
    freelist<node, Compression, PoolSize>    m_pool;
};

}

#endif