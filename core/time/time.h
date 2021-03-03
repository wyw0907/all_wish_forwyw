#ifndef WISH_TIME_H
#define WISH_TIME_H
#include "../common.h"
namespace wish {
    class time_util 
    {
    public:
        static constexpr uint64_t NANOSECONDS_PER_MILLISECOND = 1000 * 1000;
    
        static uint64_t timestamp();
        
        static uint64_t mtimestamp();
        
        static uint64_t utimestamp();
        
        static uint64_t ntimestamp();
        
        static std::string current_time(const char *fmt); 

        static std::string current_time();
    };


}

#endif