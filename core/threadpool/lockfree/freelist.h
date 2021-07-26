#ifndef WISH_ALLOCATE_H
#define WISH_ALLOCATE_H

#include "tagged_ptr.h"
#include <atomic>
#include <memory>
#include <cstring>
namespace wish
{

template<class T, bool Compression, size_t PoolSize, class Alloca = std::allocator<T>>
class freelist : Alloca
{
    struct node
    {
        tagged_ptr<node, Compression> next;
    };
    using tagged_node_ptr = tagged_ptr<node, Compression>;
    using atomic_tagged_node_ptr = std::atomic<tagged_node_ptr>;
public:
    freelist() :
        m_pool(tagged_node_ptr(nullptr, 0))
    {
        for (size_t i = 0; i < PoolSize; ++i)
        {
            T *t = Alloca::allocate(1);
            memset((void *)t, 0, sizeof(T));
            destruct(t);
        }
    }
    template <class... Args>
    T *construct(Args&&... args)
    {
        T * t = allocate_safe();
        if (t)
        {
            new (t) T(std::forward<Args>(args)...);
        }
        return t;
    }

    void destruct(T* t)
    {
        t->~T();
        deallocate_safe(t);
    }
private:
    T *allocate_safe()
    {
        auto old = m_pool.load(std::memory_order_acquire);
        for (;;)
        {
            if (!old.get_ptr())
            {
                T *ptr = Alloca::allocate(1);
                memset((void *)ptr, 0, sizeof(T));
                return ptr;
            }
            else
            {
                tagged_node_ptr next_pool(old->next.get_ptr(), old.get_next_tag());
                if (m_pool.compare_exchange_weak(old, next_pool, std::memory_order_acq_rel))
                {
                    return reinterpret_cast<T*>(old.get_ptr());
                }
            }
        }
    }

    void deallocate_safe(T *t)
    {
        auto old = m_pool.load(std::memory_order_acquire);
        auto n =  reinterpret_cast<node *>(t);
        for (;;)
        {
            tagged_node_ptr new_pool(n, old.get_tag());
            new_pool->next.set_ptr(old.get_ptr()); // i know this
            if (m_pool.compare_exchange_weak(old, new_pool, std::memory_order_acq_rel))
            {
                return;
            }
        }
    }

    atomic_tagged_node_ptr m_pool;
};
template<class T, bool Compression>
class freelist<T, Compression, 0, std::allocator<T>>
{
public:
    freelist()
    {
    }
    template <class... Args>
    T *construct(Args&&... args)
    {
        return new T(std::forward<Args>(args)...);
    }
    void destruct(T* t)
    {
        delete t;
    }
};

}
#endif