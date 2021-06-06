#ifndef WISH_HTTP_URI_H
#define WISH_HTTP_URI_H
#include "../../util/string_util.h"
#include <map>
// foo://username:password@example.com:808/api/index.htm?type=a&name=b#nose
// \_/   \_______________/ \_________/ \_/     \___/ \_/ \___________/ \__/
//  |           |               |       |        |    |        |         |
//  |       user_info          host    port      |    |       query fragment
//schema \_______________________________/\______|____|/
//                      |                     |  |    |
//                  authority               path |    |
//                                        \______|____|___________________/
//                                               |    |     |
//                          interpretable as filename |    url
//                                           interpretable as extension

namespace wish
{
    namespace http
    {
        class uri
        {
        public:
            bool parse_from_array(const char *url);

            const std::string &host() const;
            const std::string &path() const;
            const std::string &user_info() const;
            const std::string &schema() const;
            const std::string &fragment() const;
            const std::string &query() const;
            const std::string &url() const;
            const std::map<std::string, std::string> &params() const;
            uint16_t port() const;

        private:
            std::string m_host;
            uint16_t m_port;
            std::string m_path;
            std::string m_user_info;
            std::string m_schema;
            std::string m_fragment;
            std::string m_query;
            std::map<std::string, std::string> m_query_map;
            std::string m_url;
        };
    }
}

#endif