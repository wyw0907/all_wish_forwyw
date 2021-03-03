#include "crypt.h"
#ifdef _LINUX
#include <openssl/md5.h>
#endif
namespace wish {
    namespace crypt {
        
        std::string encrypt_md5(const char *src, size_t len)
        {
#ifdef _LINUX
            uint8_t md5str[16] = {0};
            char buf[512] = {0};
            MD5(reinterpret_cast<const uint8_t *>(src), len, md5str);
            
            for (int i = 0; i < 16; ++i)
            {
                sprintf(buf + i * 3, "%2.2x", md5str[i]);
            }
            return buf;
#else
            return "NOSUPPORTFORWINDOS";
#endif
        }

    } // namespace crypt
} 

