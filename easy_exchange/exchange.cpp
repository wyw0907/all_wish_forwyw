#include "exchange.h"

EasyExchange::EasyExchange()
{
    size_t i = 0;
    while(i++ < 30)
    {
        gen_tick(i);
    }
}

EasyExchange::~EasyExchange()
{
}

void EasyExchange::gen_tick(size_t i)
{
    Tick t;
    t.init(i);
    std::list<Tick> _ltick;
    _ltick.push_back(t);
    auto key = generate_code_key(t.stock);
    std::cout << key << std::endl;
    m_quote_tick[key] = std::move(_ltick);
}

void EasyExchange::start()
{
    
    Runnable::start();
}

void EasyExchange::run()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        m_deal_thread.insert([this] () {
            this->on_update_tick();
        });
    }
}

void EasyExchange::on_update_tick()
{
    for (auto&v : m_quote_tick)
    {
        auto tick = v.second.back();
        srand(time(NULL));
        tick.last_price = rand() % 10000 / 100;
        v.second.push_back(tick);
    }
}

void EasyExchange::wait_input()
{
    char c;
    while (std::cin >> c)
    {
        m_deal_thread.insert([this, c] () {
            this->on_command(c);
        });
    }
}

void EasyExchange::on_command(char c)
{
    m_command_buf.append(1, c);
    if (m_command_buf.back() == '\n' || m_command_buf.back() == '\r')
    {
        do 
        {
            char op[32] = {0};
            uint16_t market = 0;
            char code[32] = {0};
            int volume = 0;
            sscanf(m_command_buf.c_str(), "%s %d %s %d", &op, &market, &code, &volume);
            if (market == 0 || strlen(code) != 6)
            {
                m_notice = "invalid code or market";
                m_notice_level = NoticeLevel::Error;
                break;
            }
            if (volume < 100 || volume % 100 != 0)
            {
                m_notice = "invalid volume";
                m_notice_level = NoticeLevel::Error;
                break;
            }
            CodeKey key = 0;
            for (auto&stock : s_vec_stock)
            {
                if (stock.market == market && stock.code == code)
                {
                    key = generate_code_key(stock);
                    if (!m_quote_tick.count(key))
                    {
                        key = 0;
                    }
                    break;
                }
            }
            if (key == 0)
            {
                m_notice = "invalid code or market";
                m_notice_level = NoticeLevel::Error;
                break;
            }
            if (strcmp(op, "buy") == 0)
            {
                if(!m_account.buy_stock(key, m_quote_tick[key].back().last_price, volume))
                {
                    m_notice = "余额不足";
                    m_notice_level = NoticeLevel::Warn;
                }
            }
            else if (strcmp(op, "sell") == 0)
            {
                if(!m_account.sell_stock(key, m_quote_tick[key].back().last_price, volume))
                {
                    m_notice = "持仓不足";
                    m_notice_level = NoticeLevel::Warn;
                }
            }
            else
            {
                m_notice = "invalid operate";
                m_notice_level = NoticeLevel::Error;
                break;
            }
        }while(0);
        m_command_buf.clear();
    }
}

void EasyExchange::show()
{
    printf("\033[31A");
    printf("|%8s\t|%10s\t|%8s\t|%8s\t|\n", "股票代码", "股票名词", "最新价", "持仓");
    for (auto&v : m_quote_tick)
    {
        auto& tick = v.second.back();
        printf("\033[41m|%8s\t|%10s\t|%5.2lf\t|%8d\t|\033[0m\n", 
            tick.stock.code.c_str(), 
            tick.stock.stockname.c_str(), 
            tick.last_price, 
            m_account.get_position(tick.code_key));
    }
    printf("\n");
    printf("您的账户余额 : %lf", m_account.get_avaliable());
    if (!m_notice.empty())
    {
        switch (m_notice_level)
        {
        case NoticeLevel::Info:
            printf("提示：%s\n", m_notice.c_str());
            break;
        case NoticeLevel::Warn:
            printf("\033[33m注意：%s\033[0m\n", m_notice.c_str());
            break;
        case NoticeLevel::Error:
            printf("\033[31m错误：%s\033[0m\n", m_notice.c_str());
            break;
        default:
            printf("\n");
            break;
        }
    }
    else
    {
        printf("\n");
    }
    printf("请输入>>>%s", m_command_buf.c_str());
}