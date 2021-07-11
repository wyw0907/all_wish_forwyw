#pragma once
#include <iostream>
#include <map>
#include <list>
#include <thread>
#include <chrono>
#include "quote_def.h"
#include "asset_def.h"
#include "log/log.h"
#include "thread/runnable.h"
#include "thread/threadpool.h"

enum class NoticeLevel
{
    Nil,
    Info,
    Warn,
    Error,
};
class EasyExchange : public Runnable
{
public:
    EasyExchange();
    ~EasyExchange();
    void start() override;

    void wait_input();
private:

    void gen_tick(size_t i);

    void run() override;

    void on_update_tick();

    void on_command(char c);

    void show();

    std::map<CodeKey, std::list<Tick>>  m_quote_tick;
    Account                             m_account;
    ThreadPool                          m_deal_thread{1};
    std::string                         m_command_buf;
    std::string                         m_notice;
    NoticeLevel                         m_notice_level;
};