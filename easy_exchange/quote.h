
#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

using MarketType = uint8_t;

#define IS_EXCHANGE_SZ(x) ((x) & 0x10)
#define IS_EXCHANGE_SH(X) ((X) & 0X20)


// 前8位表示交易所，后24位表示代码
using CodeKey = uint32_t;

struct Stock
{
    MarketType  market;
    std::string code;
    std::string stockname;
};

static CodeKey generate_code_key(const Stock& _stock)
{
    CodeKey key = 0;
    key |= _stock.market >> 4;
    key <<= 24;
    key |= std::stoul(_stock.code);
    return key;
}

struct Tick
{
    Stock  stock;
    double last_price = 0.0;
    double yesterday_close_price = 0.0;
    uint64_t volume = 0; //买卖总手
    double buy_price[10] = {0.0};
    uint64_t buy_volume[10] = {0};
    double sell_price[10] = {0.0};
    uint64_t sell_volume[10] = {0};
    CodeKey code_key = 0;

    void init(size_t i);
    // 更新行情
    void update();

private:
    // 获取一个-0.10~0.10的数
    double get_randx();
};


static std::unordered_map<MarketType, std::string> s_map_marketname{
    {0x11, "深圳A"},
    {0x12, "深圳B"},
    {0x13, "深圳ST"},
    {0x21, "上海A"},
    {0x22, "上海B"},
    {0x23, "上海ST"}
};

static std::vector<Stock> s_vec_stock{
    {0x21, "600001", "安全银行"},
    {0x21, "600002", "平安国际"},
    {0x21, "600003", "xx理想"},
    {0x22, "600010", "天天向上"},
    {0x22, "600011", "爱情公寓"},
    {0x23, "600021", "ST*爱"},
    {0x11, "300033", "童话书"},
    {0x11, "300001", "琉璃国"},
    {0x11, "300010", "梦想乐园"},
    {0x12, "000009", "九九九二"},
    {0x12, "000019", "大家好"},
    {0x13, "000099", "ST*第九"},

    {0x21, "600101", "安全银行一"},
    {0x21, "600102", "平安国际一"},
    {0x21, "600103", "xx理想一"},
    {0x22, "600110", "天天向上一"},
    {0x22, "600111", "爱情公寓一"},
    {0x23, "600121", "ST*爱一"},
    {0x11, "300133", "童话书一"},
    {0x11, "300101", "琉璃国一"},
    {0x11, "300110", "梦想乐园一"},
    {0x12, "000109", "九九九二一"},
    {0x12, "000119", "大家好一"},
    {0x13, "000199", "ST*第九一"},

    {0x21, "600201", "安全银行二"},
    {0x21, "600202", "平安国际二"},
    {0x21, "600203", "xx理想二"},
    {0x22, "600210", "天天向上二"},
    {0x22, "600211", "爱情公寓二"},
    {0x23, "600221", "ST*爱二"},
    {0x11, "300233", "童话书二"},
    {0x11, "300201", "琉璃国二"},
    {0x11, "300210", "梦想乐园二"},
    {0x12, "000209", "九九九二二"},
    {0x12, "000219", "大家好二"},
    {0x13, "000299", "ST*第九二"},

};

