#ifndef WISH_MATRIX_H
#define WISH_MATRIX_H
#include <asio.hpp>
namespace wish
{
    class i_matrix 
    {
    public: 
        virtual void on_receive(uint64_t id, const char *data, size_t size) = 0;
        virtual void on_receive_error(uint64_t id, const asio::error_code& ec) {}
        virtual void on_send_success(uint64_t id, size_t size) {}
        virtual void on_send_error(uint64_t id, const asio::error_code& ec) {}
    };
}

#endif