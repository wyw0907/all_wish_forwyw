#ifndef WISH_HTTP_STRUCT_H
#define WISH_HTTP_STRUCT_H
#include <map>
#include <string>
namespace wish
{
    namespace http
    {
        using header_type = std::map<std::string, std::string>;
        using params_type = std::map<std::string, std::string>;

        struct request
        {
            std::string version;
            // url = method + path + params
            std::string url;
            std::string method;
            std::string path;
            params_type params;
            header_type headers;
            std::string body;
        };

        struct response
        {
            std::string version;
            uint32_t status;
            header_type headers;
            std::string body;
        };
    }
} // namespace wish


#endif