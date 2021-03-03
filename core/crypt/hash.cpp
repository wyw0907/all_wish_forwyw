#include "crypt.h"
#include "MurmurHash3.h"
namespace wish {
    namespace crypt {
        
        uint32_t hash_32(const char *src, size_t len)
        {
            uint32_t h;
            MurmurHash3_x86_32(src, len, 4, &h);
            return h;
        }

    } // namespace crypt
} 

