#include "../../deps/cppzmq/zmq.hpp"
#include "../../deps/cppzmq/zmq_addon.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_PUSH);
    socket.bind("tcp://127.0.0.1:8003");
    zmq_pollitem_t items[] = {{socket.handle(), 0, ZMQ_POLLIN, 0}};
    while (true)
    {
        try
        {
            static int idx = 0;
            zmq_sleep(1);
            std::string rep = "hellow world " + std::to_string(idx++);
            socket.send(zmq::message_t(rep.c_str(), rep.size()), zmq::send_flags::none);
        }
        catch (const zmq::error_t &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}