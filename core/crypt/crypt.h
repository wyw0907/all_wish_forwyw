#ifndef WISH_CRYPT_H
#define WISH_CRYPT_H
#include "../common.h"

namespace wish {
    namespace crypt {
        
        std::string encrypt_md5(const char *src, size_t len);

    } // namespace crypt
} 


#endif

