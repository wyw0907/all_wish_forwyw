#ifndef WISH_SPIN_LOCK_H
#define WISH_SPIN_LOCK_H
#include <atomic>
namespace wish
{
    class spin_lock
    {
    public:
        spin_lock() = default;
        ~spin_lock() = default;
        spin_lock(const spin_lock &) = delete;
        spin_lock &operator=(const spin_lock &) = delete;
        void lock()
        {
            while (!m_flag.test_and_set())
            {
            }
        }

        void unlock()
        {
            m_flag.clear();
        }

    private:
        std::atomic_flag m_flag;
    };
} // namespace wish
#endif