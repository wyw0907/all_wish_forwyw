#include "../../deps/cppzmq/zmq.hpp"
#include "../../deps/cppzmq/zmq_addon.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_DEALER);
    socket.connect("tcp://127.0.0.1:8005");
    std::string input;
    zmq::message_t rsp;
    while(std::cin >> input)
    {
        socket.send(zmq::message_t(input.c_str(), input.size()), zmq::send_flags::none);
        socket.recv(rsp, zmq::recv_flags::none);
        //socket.recv(rsp);
        std::cout << rsp.to_string() << std::endl;
    }
}