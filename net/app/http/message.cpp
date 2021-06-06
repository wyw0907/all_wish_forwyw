#include "message.h"
#include <iostream>
namespace wish
{
    namespace http
    {
        http_parser_settings message::s_settting = {
            &message::on_message_begin_cb,
            &message::on_url_cb,
            &message::on_status_cb,
            &message::on_header_field_cb,
            &message::on_header_value_cb,
            &message::on_headers_complete_cb,
            &message::on_body_cb,
            &message::on_message_complete_cb,
            &message::on_chunk_header_cb,
            &message::on_chunk_complete_cb,
        };

        int message::on_message_begin_cb(http_parser *parser)
        {
            return 0;
        }
        int message::on_url_cb(http_parser *parser, const char *at, size_t length)
        {
            message *msg = static_cast<message *>(parser->data);
            return msg->on_url(at, length);
        }
        int message::on_status_cb(http_parser *parser, const char *at, size_t length)
        {
            message *msg = static_cast<message *>(parser->data);
            return msg->on_status(at, length);
        }
        int message::on_header_field_cb(http_parser *parser, const char *at, size_t length)
        {
            message *msg = static_cast<message *>(parser->data);
            return msg->on_header_field(at, length);
        }
        int message::on_header_value_cb(http_parser *parser, const char *at, size_t length)
        {
            message *msg = static_cast<message *>(parser->data);
            return msg->on_header_value(at, length);
        }
        int message::on_headers_complete_cb(http_parser *parser)
        {
            return 0;
        }
        int message::on_body_cb(http_parser *parser, const char *at, size_t length)
        {
            message *msg = static_cast<message *>(parser->data);
            return msg->on_body(at, length);
        }
        int message::on_message_complete_cb(http_parser *parser)
        {
            message *msg = static_cast<message *>(parser->data);
            return msg->on_message_complete();
        }
        int message::on_chunk_header_cb(http_parser *parser)
        {
            DLOG("on_chunk_header");
            return 0;
        }
        int message::on_chunk_complete_cb(http_parser *parser)
        {
            DLOG("on_chunk_complete");
            return 0;
        }

        message::message(http_parser_type type) : m_type(type)
        {
            http_parser_init(&m_parser, m_type);
            //http_parser_settings_init(&s_settting);
            m_parser.data = this;
        }

        bool message::parse_from_array(const char *data, size_t size)
        {
            size_t parsed = http_parser_execute(&m_parser, &s_settting, data, size);
            if (m_parser.http_errno != 0)
            {
                ELOG("parse failed: {}", std::string(data + parsed));
                return false;
            }
            return true;
        }

        bool message::is_completed() const
        {
            if( m_completed)
            {
                return true;
            }
            std::map<std::string, std::string>::const_iterator cit;
            cit = m_headers.find("Connection");
            if (cit->second == "close")
            {
                return true;
            }
            return false;
        }

        std::string message::version() const
        {
            return std::to_string(m_version.first) + "." + std::to_string(m_version.second);
        }

        const std::string &message::url() const
        {
            return m_url;
        }

        std::string message::method() const
        {
            return http_method_str(m_method);
        }

        uint32_t message::status() const
        {
            return uint32_t(m_status);
        }

        const std::map<std::string, std::string> &message::headers() const
        {
            return m_headers;
        }

        const std::string &message::body() const
        {
            return m_body;
        }

        int message::on_url(const char *at, size_t length)
        {
            m_url = std::string(at, length);
            return 0;
        }
        int message::on_status(const char *at, size_t length)
        {
            if (HTTP_RESPONSE == m_type)
            {
                m_status = http_status(m_parser.status_code);
            }
            return 0;
        }
        int message::on_header_field(const char *at, size_t length)
        {
            m_last_header_field = std::string(at, length);
            return 0;
        }
        int message::on_header_value(const char *at, size_t length)
        {
            assert(!m_last_header_field.empty());
            m_headers[m_last_header_field] = std::string(at, length);
            return 0;
        }
        int message::on_body(const char *at, size_t length)
        {
            m_body = std::string(at, length);
            return 0;
        }
        int message::on_message_complete()
        {
            m_version = std::make_pair(m_parser.http_major, m_parser.http_minor);
            if (HTTP_REQUEST == m_parser.type)
            {
                m_method = http_method(m_parser.method);
                DLOG("completed ! method[{}], version[{}.{}]",
                     http_method_str(m_method),
                     m_version.first,
                     m_version.second);
            }
            else if (HTTP_RESPONSE == m_parser.type)
            {
                DLOG("completed ! status[{}], version[{}.{}]",
                     http_status_str(m_status),
                     m_version.first,
                     m_version.second);
            }
            // parser只能解析相同type，如果是BOTH，它会以第一次解析到的type为准，
            // 所以如果当type=HTTP_BOTH时需要复用，这里需要重置parser
            if (HTTP_BOTH == m_type)
            {
                http_parser_init(&m_parser, m_type);
            }
            m_completed = true;
            return 0;
        }
    }
}