#include "uri.h"
#include <iostream>
namespace wish
{
    namespace http
    {
        static void skip_space(const char *p)
        {
            while (*p && *p == ' ')
                p++;
        }
        bool uri::parse_from_array(const char *url)
        {
            const char *p = url;
            skip_space(p);
            const char *start = p;
            // 先处理头部可能出现的schema和userinfo
            for (; *p; ++p)
            {
                skip_space(p);
                if (*p == ':')
                {
                    if (p[1] == '/' && p[2] == '/')
                    {
                        m_schema.assign(start, p - start);
                        p += 2;
                        start = p + 1;
                    }
                }
                else if (*p == '@')
                {
                    m_user_info.assign(start, p - start);
                    start = p + 1;
                }
            }
            p = start;
            // 处理 address [.+][:?][.*][/?]
            // 找到/之前最后一个:
            const char *addr_start = p;
            const char *addr_end = nullptr;
            const char *split_pos = nullptr;

            for (; *p; ++p)
            {
                // std::cout << p << std::endl;;
                skip_space(p);
                if (*p == '/' || *p == '?' || *p == '#')
                {
                    addr_end = p;
                    break;
                }
                else if (*p == ':')
                {
                    split_pos = p;
                }
            }
            if (addr_end == nullptr)
            {
                addr_end = p;
            }
            if (split_pos == nullptr)
            {
                m_host.assign(addr_start, addr_end - addr_start);
                m_port = 80;
            }
            else
            {
                m_host.assign(addr_start, split_pos - addr_start);
                m_port = atoi(std::string(split_pos + 1, addr_end - split_pos + 1).c_str());
            }
            const char *url_start = p;
            do
            {
                if (!*p || (*p != '/' && *p != '?' && *p != '#'))
                {
                    break;
                }
                start = ++p;
                for (; *p && *p != '?' && *p != '#'; ++p)
                {
                }
                m_path.assign(start, p - start);

                if (!*p || (*p != '?' && *p != '#'))
                {
                    break;
                }
                start = ++p;
                for (; *p && *p != '#'; ++p)
                {
                }
                m_query.assign(start, p - start);
                if (!m_query.empty())
                {
                    auto rets = wish::split(m_query, '&');
                    for (auto &ret : rets)
                    {
                        auto kv = wish::split(ret, '=');
                        if (kv.size() != 2)
                        {
                            return false;
                        }
                        m_query_map.insert({kv[0], kv[1]});
                    }
                }

                if (!*p || *p != '#')
                {
                    break;
                }
                start = ++p;
                for (; *p; ++p)
                {
                }
                m_fragment.assign(start, p - start);
            } while(0);
            m_url.assign(url_start, p - url_start);
            return true;
        }

        const std::string &uri::host() const
        {
            return m_host;
        }
        const std::string &uri::path() const
        {
            return m_path;
        }
        const std::string &uri::user_info() const
        {
            return m_user_info;
        }
        const std::string &uri::schema() const
        {
            return m_schema;
        }
        const std::string &uri::fragment() const
        {
            return m_fragment;
        }
        const std::string &uri::query() const
        {
            return m_query;
        }
        const std::string &uri::url() const
        {
            return m_url;
        }
        const std::map<std::string, std::string> &uri::params() const
        {
            return m_query_map;
        }
        uint16_t uri::port() const
        {
            return m_port;
        }
    }
}