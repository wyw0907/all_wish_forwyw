
#include "net/app/easy_tcp/client.h"
#include <iostream>
int main()
{
    wish::easy_tcp::client cli;
    cli.regist_recv_cb([] (const char *data, size_t size) {
        std::cout << "recv_cb : " << std::string(data, size) << std::endl;
    });
    if(!cli.sync_connect("127.0.0.1", 5050))
    {
        std::cout << "connect fail" << std::endl;
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