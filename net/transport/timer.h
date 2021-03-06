#include <asio/steady_timer.hpp>
namespace wish
{
    class timer
    {
    public:
        using tid = uint16_t;
        class timer_data
        {
        public:
            enum timer_status : uint8_t
            {
                TIMER_CREATED,
                TIMER_STARTED,
                TIMER_CANCELD,
            };

            timer_data(tid id, std::shared_ptr<asio::io_context> io_ctx) : m_id(id), m_timer(io_ctx.get())
            {
            }
            bool operator==(const timer_data& other)
            {
                return m_id == other.m_id;
            }
            tid m_id;
            uint64_t m_interval; //ms
            asio::steady_timer m_timer;
        };


    public:
        timer(std::shared_ptr<asio::io_context> io_ctx) : m_io_context(io_ctx)
        {

        }

        ~timer()
        {
            stop_all();
        }

        // return -1 if error
        tid start_timer(uint64_t interval, std::function<bool(tid)> cb)
        {
            
        }

        bool start_timer(tid id, uint64_t interval, std::function<bool(tid)> cb)
        {
            
        }

        timer_data find_timer()
        {

        }

        void stop_all();

        std::shared_ptr<asio::io_context> m_io_context;

    };
} // namespace wish