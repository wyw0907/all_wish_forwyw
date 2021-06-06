#ifndef WISH_MATRIX_H
#define WISH_MATRIX_H
#define ASIO_STANDALONE
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include "../util/log.h"
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