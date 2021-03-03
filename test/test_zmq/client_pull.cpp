#include "../../deps/cppzmq/zmq.hpp"
#include "../../deps/cppzmq/zmq_addon.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_PULL);
    socket.connect("tcp://127.0.0.1:8003");
    std::string input;
    zmq::message_t rsp;

    while(true)
    {
        socket.recv(rsp);
        std::cout << rsp.to_string() << std::endl;
    }
}