#include "net/transport/tcp/client.h"
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
static int get_thread_id()
{
    thread_local const int tid = static_cast<size_t>(::syscall(SYS_gettid));
    return tid;
}

class client : public wish::tcp::client<wish::tcp::session<wish::tcp::matrix>, wish::tcp::matrix>
{
public:
    void on_receive(uint64_t id, const char *data, size_t size) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_receive: " << std::string(data, size) << std::endl;
    }
    void on_receive_error(uint64_t id, const asio::error_code &ec) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_receive_error: " << ec.message() << std::endl;
    }
    void on_send_success(uint64_t id, size_t size) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_send_success: " << size << std::endl;
    }
    void on_send_error(uint64_t id, const asio::error_code &ec) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_send_error: " << ec.message() << std::endl;
    }

    void on_connect(uint64_t id) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_connect" << std::endl;
    }
    void on_disconnect(uint64_t id) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_disconnect" << std::endl;
    }
    void on_connect_fail(uint64_t id) override
    {
        std::cout << "[" << id << "]"
                  << " thread[" << get_thread_id() << "] on_connect_fail" << std::endl;
    }
};


int main()
{
    client cli;
    if(!cli.connect("127.0.0.1", 6666))
    {
        return 0;
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