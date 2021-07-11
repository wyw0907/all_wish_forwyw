#pragma once
#include "quote_def.h"

class Account
{
public:
    bool buy_stock(CodeKey code, double price, uint64_t volume)
    {
        double balance = price * volume;
        double old = m_fund.load();
        while(!m_fund.compare_exchange_weak(old, old - balance))
        {
            if (old < old - balance)
            {
                return false; //交易失败
            }
        }
        while(m_op_flag.test_and_set())
        {
            if (m_position.count(code))
            {
                m_position[code] += volume;
            }
            else
            {
                m_position.insert({code, volume});
            }
        }
        m_op_flag.clear();
        return true;
    }

    bool sell_stock(CodeKey code, double price, uint64_t volume)
    {
        while(m_op_flag.test_and_set())
        {
            if (m_position.count(code))
            {
                auto& position = m_position[code];
                
                if (position > volume)
                {
                    position -= volume;
                }
                else if(position == volume)
                {
                    m_position.erase(code);
                }
                else
                {
                    m_op_flag.clear();
                    return false;
                }
            }
            else
            {
                m_op_flag.clear();
                return false;
            }
        }
        m_op_flag.clear();

        double balance = price * volume;
        double old = m_fund.load();
        while(!m_fund.compare_exchange_weak(old, old + balance));
        return true;
    }

    double get_avaliable() const
    {
        return m_fund.load();
    }

    std::map<CodeKey, uint64_t> get_position() const
    {
        std::map<CodeKey, uint64_t>  ret;
        while(m_op_flag.test_and_set())
        {
           ret = m_position;
        }
        m_op_flag.clear();
        return ret;
    }

    uint64_t get_position(CodeKey key) const
    {
        uint64_t position = 0;
        while(m_op_flag.test_and_set())
        {
            if (m_position.count(key))
            {
                position = m_position.at(key);
            }
        }
        m_op_flag.clear();
        return position;
    }


private:
    mutable std::atomic_flag    m_op_flag;
    std::map<CodeKey, uint64_t> m_position; //持仓
    std::atomic<double>         m_fund{0.0};   // 资金

    std::string m_userid;
    std::string m_password;
};