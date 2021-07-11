#include "quote.h"

void Tick::init(size_t i)
{
    stock = s_vec_stock[i];
    srand(time(NULL));
    yesterday_close_price = rand() % 10000 / 100;

    code_key = generate_code_key(stock);
}

// 更新行情
void Tick::update()
{
    if (last_price == 0)
    {
        last_price = yesterday_close_price;
    }
    last_price += last_price * get_randx();
}

// 获取一个-0.10~0.10的数
double Tick::get_randx()
{
    auto x = double(std::rand() % 9 + 1) / 100;
    return std::rand() % 2 ? x : -x;
}