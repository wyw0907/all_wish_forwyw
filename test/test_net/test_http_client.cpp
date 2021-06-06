#include "net/app/http/client.h"
#include "net/util/json.hpp"
int main()
{
    std::string id = "test-123456";
    spdlog::set_level(spdlog::level::debug);
    nlohmann::json _json = {
        {"ID", id},
        {"Name", "test"},
        {"Port", 6060},
        {"Address", "127.0.0.1"},
        {"Meta", {
                     {"protocol", "http"},
                     {"transport", "tcp"},
                 }}};
    _json["Tags"].push_back("v1");
    _json["Tags"].push_back("public");

    //_json["Check"] = {
    //    {"TTL", "15s"},
    //    {"DeregisterCriticalServiceAfter", "1m"},
    //};

    _json["Check"] = {
        {"GRPC", "10.10.120.4:9999/test"},
        {"Interval", "5s"},
        {"DeregisterCriticalServiceAfter", "1m"},
    };

    std::string body = _json.dump();
    wish::http::header_type headers;
    headers["Host"] = "10.10.120.4:8500";
    headers["Accept"] = "*/*";
    headers["Content-Type"] = "application/x-www-form-urlencoded";
    try
    {
        wish::http::client cli;
        auto rsp = cli.put("http://10.10.120.4:8500/v1/agent/service/register?replace-existing-checks=true", headers, body);
        ILOG("version: {}", rsp.version);
        ILOG("status: {}", rsp.status);
        for (auto &v : rsp.headers)
        {
            ILOG("head {}: {}", v.first, v.second);
        }
        ILOG("body: {}", rsp.body);
    }
    catch (const std::exception &e)
    {
        ELOG("exception: {}", e.what());
    }
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        try
        {
            wish::http::client cli;
            auto rsp = cli.put(std::string("http://10.10.120.4:8500/v1/agent/check/pass/service:") + id, headers, "");
            ILOG("version[{}] status[{}] body[{}]", rsp.version, rsp.status, rsp.body);
        }
        catch (const std::exception &e)
        {
            ELOG("exception: {}", e.what());
        }
    }
}