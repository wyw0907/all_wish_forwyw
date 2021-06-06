
#include "net/app/easy_tcp/client.h"
#include <iostream>
int main()
{
    wish::easy_tcp::client cli;
    cli.regist_recv_cb([] (const char *data, size_t size) {
        ILOG("recv_cb : {}", std::string(data, size).c_str());
    });
    if(!cli.sync_connect("127.0.0.1", 5050))
    {
        FLOG("connect fail");
        cli.shutdown();
        return -1;
    }
    std::string input;
    while(std::cin >> input)
    {
        if (input.at(0) == 'q')
        {
            break;
        }
        cli.send_msg(input);
    }
}