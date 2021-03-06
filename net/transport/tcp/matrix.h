#ifndef WISH_TCP_MATRIX_H
#define WISH_TCP_MATRIX_H
#include <asio.hpp>
#include "../matrix.h"
namespace wish
{
    namespace tcp
    {
        class matrix : public i_matrix
        {
        public:
            //void on_receive(uint64_t id, const char *data, size_t size) = 0;
            //void on_receive_error(uint64_t id, const asio::error_code& ec) {}
            //void on_send_seccuess(uint64_t id, size_t size) {}
            //void on_send_error(uint64_t id, cosnt asio::error_code& ec) {}
            virtual void on_connect(uint64_t id) {}
            virtual void on_disconnect(uint64_t id) {}
            virtual void on_connect_fail(uint64_t id) {}
        };
    }

}

#endif