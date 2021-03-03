/*
nanomsg push/pull 管道模式， 1对多，
push为服务端，pull为客户端
*/

#include "nanomsg/src/nn.h"
#include "nanomsg/src/ipc.h"
#include "nanomsg/src/pipeline.h"
#include "core/log.h"
#include <thread>
using namespace std::chrono_literals;

#define IPC_ADDR "inproc://pipeline.ipc"
#define TCP_ADDR "tcp://127.0.0.1:7777"
int main(int argc, char **argv)
{
    std::string url = IPC_ADDR;
    if (argc >= 2)
    {
        for (int i = 0; i< argc; ++i)
        {
            if (0 == strcmp(argv[i], "--ipc"))
            {
                url = IPC_ADDR;
            }
            else if (0 == strcmp(argv[i], "--tcp"))
            {
                url = TCP_ADDR;
            }
        }
    }

    auto sock = nn_socket(AF_SP, NN_PUSH);
    if (sock < 0)
    {
        int code = nn_errno();
        LOG_ERR("nn_socket failed : %s(%d) ", nn_strerror(code), code);
        return sock;
    }

    if (0 > nn_bind(sock, url.c_str()))
    {
        int code = nn_errno();
        LOG_ERR("nn_bind failed : %s(%d) ", nn_strerror(code), code);
        return -1;
    }
    for (int i = 0; i < 100; i++)
    {
        std::this_thread::sleep_for(1s);
        std::string ss = "push 消息 " + std::to_string(i);
        int len = nn_send(sock, ss.data(), ss.size(), 0);
        if (len < 0)
        {
            int code = nn_errno();
            LOG_ERR("nn_send failed : %s(%d)", nn_strerror(code), code);
        }
        LOG_DBG("send %d : %s", len, ss.c_str());
    }

    nn_close(sock);
}