
#include "net/tcp/server.h"
#include <iostream>
int main()
{   
    wish::simple_server srv;
    wish::simple_session::handle_msg([&srv] (uint64_t id, const char *msg, size_t size) {
        std::cout << "handle msg : " << std::string(msg, size) << std::endl;
        std::string rsp("i recv: ");
        rsp += std::string(msg, size);
        srv.send_msg(id, rsp);
    });

    srv.set_addr(5050);
    srv.start();
    srv.wait();
}