
#include "net/tcp/client.h"
#include <iostream>
int main()
{
    wish::simple_client cli;
    wish::simple_session::handle_msg([&cli] (uint64_t id, const char *msg, size_t size) {
        std::cout << "handle msg : " << std::string(msg, size) << std::endl;
    });

    if(!cli.connect("127.0.0.1", 5050))
    {
        std::cout << "connect fail" << std::endl;
    }

    cli.send_msg("hello world!");

    cli.wait();
}