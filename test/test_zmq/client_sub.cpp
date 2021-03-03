#include "../../deps/cppzmq/zmq.hpp"
#include "../../deps/cppzmq/zmq_addon.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_SUB);
    socket.connect("tcp://127.0.0.1:8004");
    std::string input;
    zmq::message_t recv_topic;
    zmq::message_t rsp;
    std::string topic = "topic_1";
    //socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    socket.set(zmq::sockopt::subscribe, topic.c_str());
    while(true)
    {
        socket.recv(recv_topic,zmq::recv_flags::none);
        socket.recv(rsp);
        std::cout << recv_topic.to_string() << " : " << rsp.to_string() << std::endl;
    }
}