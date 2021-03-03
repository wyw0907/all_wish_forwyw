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

#define IPC_ADDR "ipc:///tmp/pipeline.ipc"
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

    auto sock = nn_socket(AF_SP, NN_PULL);
    if (sock < 0)
    {
        int code = nn_errno();
        LOG_ERR("nn_socket failed : %s(%d) ", nn_strerror(code), code);
        
        return sock;
    }

    if (0 > nn_connect(sock, url.c_str()))
    {
        int code = nn_errno();
        LOG_ERR("nn_connect failed : %s(%d) ", nn_strerror(code), code);
        
        return -1;
    }

    while(true)
    {
        std::this_thread::sleep_for(1s);
        std::string ss;
        ss.resize(1000);
        int len = nn_recv(sock, const_cast<char*>(ss.data()), ss.size(), 0);
        if (len < 0)
        {
            int code = nn_errno();
            LOG_ERR("nn_recv failed : %s(%d)", nn_strerror(code), code);
        }
        LOG_DBG("recv %d : %s", len, ss.c_str());
    }

    nn_close(sock);
}