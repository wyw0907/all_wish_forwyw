#ifndef WISH_HTTP_MESSAGE_H
#define WISH_HTTP_MESSAGE_H
#include <map>
#include <string>
#include <memory>
#include "parser/http_parser.h"
#include "../../util/log.h"
namespace wish
{
    namespace http
    {
        class message
        {
        public:
            message(http_parser_type type = HTTP_BOTH);
            virtual ~message() = default;

            bool parse_from_array(const char *, size_t);

            bool is_completed() const;
            std::string version() const;
            const std::string& url() const;
            std::string method() const;
            uint32_t status() const;
            const std::map<std::string, std::string>& headers() const;
            const std::string &body() const;

            int on_url(const char *at, size_t length);
            int on_status(const char *at, size_t length);
            int on_header_field(const char *at, size_t length);
            int on_header_value(const char *at, size_t length);
            int on_body(const char *at, size_t length);
            int on_message_complete();

            static int on_message_begin_cb(http_parser *parser);
            static int on_url_cb(http_parser *parser, const char *at, size_t length);
            static int on_status_cb(http_parser *parser, const char *at, size_t length);
            static int on_header_field_cb(http_parser *parser, const char *at, size_t length);
            static int on_header_value_cb(http_parser *parser, const char *at, size_t length);
            static int on_headers_complete_cb(http_parser *parser);
            static int on_body_cb(http_parser *parser, const char *at, size_t length);
            static int on_message_complete_cb(http_parser *parser);
            static int on_chunk_header_cb(http_parser *parser);
            static int on_chunk_complete_cb(http_parser *parser);

        private:
            http_parser_type m_type;
            std::string m_url;
            std::string m_last_header_field;
            std::map<std::string, std::string> m_headers;
            http_status m_status;
            http_method m_method;
            std::pair<uint16_t, uint16_t> m_version;
            std::string m_body;
            http_parser m_parser;
            bool m_completed = false;
            static http_parser_settings s_settting;
        };
    }
}

#endif