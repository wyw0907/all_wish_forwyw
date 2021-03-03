#include "../../deps/cppzmq/zmq.hpp"
#include "../../deps/cppzmq/zmq_addon.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, ZMQ_REP);
    socket.bind("tcp://127.0.0.1:8001");
    zmq_pollitem_t items[] = {{socket.handle(), 0, ZMQ_POLLIN, 0}};
    while (true)
    {
        try
        {
            zmq::poll(&items[0], 1, 1000);
            if (items[0].revents & ZMQ_POLLIN)
            {
                zmq::message_t msg;
                auto ret = socket.recv(msg);
                if (ret)
                {
                    std::cout << msg.to_string() << std::endl;
                    std::string rep = std::string("i recv ") + msg.to_string();
                    socket.send(zmq::message_t(rep.c_str(), rep.size()), zmq::send_flags::none);
                }
            }
        }
        catch (const zmq::error_t &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}