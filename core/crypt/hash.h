#ifndef WISH_HASH_H
#define WISH_HASH_H
#include "../common.h"

namespace wish {
    namespace crypt {
        
        uint32_t hash_32(const char *src, size_t len);

    } // namespace crypt
} 


#endif

