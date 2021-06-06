#include <net/app/http/uri.h>
#include <net/util/log.h>
std::vector<std::string> test_case = {
    "www.baidu.com",
    "127.0.0.1:9999",
    "wuyunwei:****@github.com:80/freewebsys/grpc-go-demog/tree/master/src",
    "https://github.com",
    "https://baijiahao.baidu.com/s?id=1693650923518620043&wfr=spider&for=pc",
    "https://github.com#test",
    "https://baijiahao.baidu.com/s?id=1693650923518620043&wfr=spider&for=pc#123",
    "127.0.0.1:9999?type=test&",
};

int main()
{
    spdlog::set_level(spdlog::level::debug);
    for (auto &cast : test_case)
    {
        wish::http::uri http_uri;
        http_uri.parse_from_array(cast.c_str());
        DLOG("schema: {}", http_uri.schema());
        DLOG("user_info: {}", http_uri.user_info());
        DLOG("host: {}", http_uri.host());
        DLOG("port: {}", http_uri.port());
        DLOG("path: {}", http_uri.path());
        DLOG("query: {}", http_uri.query());
        DLOG("fragment: {}", http_uri.fragment());
        for (const auto&v : http_uri.params())
        {
            DLOG("params {} : {}", v.first, v.second);
        }
        DLOG("url: {}", http_uri.url());
    }
}