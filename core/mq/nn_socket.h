/*
封装nanomsg c++接口
*/
#ifndef WISH_SOCKET_NN_H
#define WISH_SOCKET_NN_H
#include "core/common.h"
#include "core/log.h"
#include "nanomsg/src/nn.h"
#include "nanomsg/src/ipc.h"
#include "nanomsg/src/reqrep.h"
#include "nanomsg/src/pipeline.h"
#include "nanomsg/src/pubsub.h"
#include "nanomsg/src/pair.h"
namespace wish {
    namespace nn {
        constexpr int MAX_MSG_LENGTH = 16 * 1024;
        constexpr int DEFAULT_NOTICE_TIMEOUT = 1000;

        enum class protocol : int 
        {
            UNKNOW = -1,
            REPLY =     NN_REP,
            REQUEST =   NN_REQ,
            PUSH =      NN_PUSH,
            PULL =      NN_PULL,
            PUBLISH =   NN_PUB,
            SUBSCRIBE = NN_SUB,
        };

        inline std::string to_string(protocol p)
        {
            switch (p)
            {
            case protocol::REPLY:
                return "reply";
            case protocol::REQUEST:
                return "request";
            case protocol::PUSH:
                return "push";
            case protocol::PULL:
                return "pull";
            case protocol::PUBLISH:
                return "publish";
            case protocol::SUBSCRIBE:
                return "subscribe";
            default:
                throw std::logic_error("invalid protocol");
            }
        } 

        constexpr inline protocol to_opposite(protocol p)
        {
            switch (p)
            {
            case protocol::REPLY:
                return protocol::REQUEST;
            case protocol::REQUEST:
                return protocol::REPLY;
            case protocol::PUSH:
                return protocol::PULL;
            case protocol::PULL:
                return protocol::PUSH;
            case protocol::PUBLISH:
                return protocol::SUBSCRIBE;
            case protocol::SUBSCRIBE:
                return protocol::PUBLISH;
            default:
                throw std::logic_error("invalid protocol");
            }
        }

        enum class transport : int 
        {
            INPROC,
            IPC,
            TCP,
            WS
        };

        inline std::string to_string(transport t)
        {
            switch (t)
            {
            case transport::INPROC:
                return "inproc://";
            case transport::IPC:
                return "ipc://";
            case transport::TCP:
                return "tcp://";
            case transport::WS:
                return "ws://";
            default:
                throw std::logic_error("invalid protocol");
            }
        } 

        class socket
        {
        public:
            virtual ~socket();

            void close();

            int send(const std::string& msg, int flags = NN_DONTWAIT);

            int recv(int flags = NN_DONTWAIT);

            const std::string & last_message() const;

            void setsockopt(int level, int option, const void *optval, size_t optvallen);

            void setsockopt(int level, int option, const std::string & value);

            void setsockopt(int level, int option, int value);

            void getsockopt(int level, int option, void *optval, size_t *optvallen);

            int  getsockopt(int level, int option);

            nn::protocol get_protocol() const ;
        protected:
            socket(int domain, protocol p, int buffer_size);

            void on_error(const std::string &msg);

            int                 m_sock;
            protocol            m_protocol;
            std::vector<char>   m_buf;
            std::string         m_url;
            std::string         m_message;
        };

        class server : public socket
        {
        public:
            server(protocol p);

            server(int domain, protocol p);

            server(int domain, protocol p, int buffer_size);

            virtual ~server();

            void bind(transport t, const std::string &url /* or path*/);
        };

        class client : public socket
        {
        public:
            client(protocol p);

            client(int domain, protocol p);

            client(int domain, protocol p, int buffer_size);

            virtual ~client();

            void connect(transport t, const std::string &url /* or path*/);
        };
    }
}

#endif