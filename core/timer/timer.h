#ifndef WISH_TIMER_H
#define WISH_TIMER_H
#include "../common.h"
namespace wish
{   
    namespace timer {
        using time_point = std::chrono::steady_clock::time_point;
        
        namespace 
        {
            struct timer_id
            {
                time_point m_time_point;
                uint32_t   m_id;
            };
        }
        DECLARE_PTR(timer_id);
        inline std::ostream& operator<<(std::ostream& os, const timer_id_ptr& d)
        {
            os << "[timer_id:" << (void *)d.get() << "]";
        }

        namespace internal
        {
            class timer_manager final
            {
            public:
                timer_manager();
                ~timer_manager();
                

            };
        }
    }
}


#endif
