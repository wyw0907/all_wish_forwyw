#include "../../deps/cppzmq/zmq.hpp"
#include "../../deps/cppzmq/zmq_addon.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_PUSH);
    socket.connect("tcp://127.0.0.1:8002");
    std::string input;
    std::cout << ">";
    zmq::message_t rsp;
    while(std::cin >> input)
    {
        //zmq::send_multipart(zmq::str_buffer(input.c_str()));
        auto ret = socket.send(zmq::message_t(input.c_str(), input.size()), zmq::send_flags::dontwait);
        //auto ret2 = socket.send(zmq::message_t(input.c_str(), input.size()), zmq::send_flags::dontwait);
        if (!ret)
        {
            exit(127);    
        }
        //socket.recv(rsp);
        //std::cout << rsp.to_string() << std::endl;
        std::cout << ">";
    }
}