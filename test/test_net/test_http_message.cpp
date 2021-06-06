#include "net/app/http/message.h"

struct http_case
{
    std::string request;
    std::string response;
    std::string uri;
};

std::vector<http_case> test_case = {
    {
        "POST /api/test?type=test&app=test#123 HTTP/1.1\r\n"
        "Content-Length: 18\r\n"
        "\r\n"
        "{request=\"hellow\"}\r\n",
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 19\r\n"
        "\r\n"
        "{response=\"world!\"}\r\n",
        "curl '127.0.0.1:80/api/test' -X POST -H 'Content-Length: 18' '{request=\"hellow\"}'",
    },
    {"GET / HTTP/1.1\r\n\
Cookie: BA_HECTOR=0kah810g058lag2koi1g4b4570q; H_PS_PSSID=33513_33272_31660_33595_33570_33392_33601_26350_33567; BD_HOME=1; BD_UPN=143254; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; PSINO=5; delPer=0; BD_CK_SAM=1; COOKIE_SESSION=27_0_9_6_4_10_0_1_9_1_0_1_352022_0_0_0_1613293126_0_1615128902%7C9%231882433_3_1612940904%7C2; __yjs_duid=1_42fbaa55fe52e0952eb31cbb25227b331615113911479; BAIDUID=D6999A51AF77079820322F3589403051:FG=1; BIDUPSID=D6999A51AF770798B89C608DE7DEC4FB; PSTM=1601265831\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Host: www.baidu.com\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.2 Safari/605.1.15\r\n\
Accept-Language: zh-cn\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Connection: keep-alive\r\n\r\n",
     "HTTP/1.1 200 OK\r\n\
Set-Cookie: BDSVRTM=19; path=/, BD_HOME=1; path=/, H_PS_PSSID=33513_33272_31660_33595_33570_33392_33601_26350_33567; path=/; domain=.baidu.com\r\n\
Expires: Mon, 08 Mar 2021 02:49:39 GMT\r\n\
Cache-Control: private\r\n\
Date: Mon, 08 Mar 2021 02:49:39 GMT\r\n\
Connection: keep-alive\r\n\
Server: BWS/1.1\r\n\
Bdqid: 0xf0412bd7000008bd\r\n\
Bdpagetype: 1\r\n\
Strict-Transport-Security: max-age=172800\r\n\
Traceid: 1615171779071950695417312166645006862525\r\n\
X-Ua-Compatible: IE=Edge,chrome=1\r\n\
Content-Length: 19\r\n\
\r\n\
{response=\"world!\"}\r\n",
     "curl 'https://www.baidu.com/' \\\r\n\\
-X 'GET' \\\r\n\\
-H 'Cookie: BA_HECTOR=0kah810g058lag2koi1g4b4570q; H_PS_PSSID=33513_33272_31660_33595_33570_33392_33601_26350_33567; BD_HOME=1; BD_UPN=143254; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; PSINO=5; delPer=0; BD_CK_SAM=1; COOKIE_SESSION=27_0_9_6_4_10_0_1_9_1_0_1_352022_0_0_0_1613293126_0_1615128902%7C9%231882433_3_1612940904%7C2; __yjs_duid=1_42fbaa55fe52e0952eb31cbb25227b331615113911479; BAIDUID=D6999A51AF77079820322F3589403051:FG=1; BIDUPSID=D6999A51AF770798B89C608DE7DEC4FB; PSTM=1601265831' \\\r\n\\
-H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' \
-H 'Upgrade-Insecure-Requests: 1' \\\r\n\\
-H 'Host: www.baidu.com' \\\r\n\\
-H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.2 Safari/605.1.15' \\\r\n\\
-H 'Accept-Language: zh-cn' \\\r\n\\
-H 'Accept-Encoding: gzip, deflate, br' \\\r\n\\
-H 'Connection: keep-alive'"},
    {
        "GET /p/5960e16cecc1 HTTP/1.1\r\n\
Cookie: BAIDU_SSP_lcr=https://www.baidu.com; Hm_lpvt_0c0e9d9b1e7d617b3e6842e85b9fb068=1615129076; Hm_lvt_0c0e9d9b1e7d617b3e6842e85b9fb068=1612941130,1615128991,1615129022,1615129076; _ga=GA1.2.1232820810.1615128991; _gat=1; _gid=GA1.2.933729837.1615128991; sensorsdata2015jssdkcross=%7B%22distinct_id%22%3A%221780d309237d60-052645c3a0132d8-48193201-2764800-1780d3092381029%22%2C%22first_id%22%3A%22%22%2C%22props%22%3A%7B%22%24latest_traffic_source_type%22%3A%22%E8%87%AA%E7%84%B6%E6%90%9C%E7%B4%A2%E6%B5%81%E9%87%8F%22%2C%22%24latest_search_keyword%22%3A%22%E6%9C%AA%E5%8F%96%E5%88%B0%E5%80%BC%22%2C%22%24latest_referrer%22%3A%22https%3A%2F%2Fwww.baidu.com%22%7D%2C%22%24device_id%22%3A%221780d309237d60-052645c3a0132d8-48193201-2764800-1780d3092381029%22%7D; locale=zh-CN; signin_redirect=https://www.jianshu.com/p/5960e16cecc1; sajssdk_2015_cross_new_user=1; __yadk_uid=UqbVJ5FY9B6HNOgxEwwbJ1CS8mNLpJ6G\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Host: www.jianshu.com\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.2 Safari/605.1.15\r\n\
Accept-Language: zh-cn\r\n\
Referer: https://www.baidu.com/link?url=f341HzRjG3imuqCPpHd-BmAeKmP_lMtx46n-B79TTWG2av7pJAA2-boDIILM8IX9&wd=&eqid=d36fd543002bccdc000000046044e97d\r\n\
Connection: keep-alive\r\n\r\n",
        "HTTP/1.1 200 OK\r\n\
Content-Type: text/html; charset=utf-8\r\n\
X-XSS-Protection: 1;mode=block\r\n\
Content-Encoding: gzip\r\n\
Date: Sun, 07 Mar 2021 14:58:21 GMT\r\n\
Connection: keep-alive\r\n\
X-Content-Type-Options: nosniff\r\n\
ETag: \"1b7c4-h1cDuHjWGIkB9C+n0OcdWcd3qaM\"\r\n\
Vary: Accept-Encoding\r\n\
X-Frame-Options: allow-from https://tongji.baidu.com\r\n\
Server: Tengine\r\n\
Strict-Transport-Security: max-age=31536000; includeSubDomains; preload\r\n\
x-envoy-upstream-service-time: 121\r\n\
Content-Length: 2\r\n\r\n11\r\n",
        "curl 'https://www.jianshu.com/p/5960e16cecc1' \r\n\\\r\n\\
-X 'GET' \\\r\n\\
-H 'Cookie: BAIDU_SSP_lcr=https://www.baidu.com; Hm_lpvt_0c0e9d9b1e7d617b3e6842e85b9fb068=1615129076; Hm_lvt_0c0e9d9b1e7d617b3e6842e85b9fb068=1612941130,1615128991,1615129022,1615129076; _ga=GA1.2.1232820810.1615128991; _gat=1; _gid=GA1.2.933729837.1615128991; sensorsdata2015jssdkcross=%7B%22distinct_id%22%3A%221780d309237d60-052645c3a0132d8-48193201-2764800-1780d3092381029%22%2C%22first_id%22%3A%22%22%2C%22props%22%3A%7B%22%24latest_traffic_source_type%22%3A%22%E8%87%AA%E7%84%B6%E6%90%9C%E7%B4%A2%E6%B5%81%E9%87%8F%22%2C%22%24latest_search_keyword%22%3A%22%E6%9C%AA%E5%8F%96%E5%88%B0%E5%80%BC%22%2C%22%24latest_referrer%22%3A%22https%3A%2F%2Fwww.baidu.com%22%7D%2C%22%24device_id%22%3A%221780d309237d60-052645c3a0132d8-48193201-2764800-1780d3092381029%22%7D; locale=zh-CN; signin_redirect=https://www.jianshu.com/p/5960e16cecc1; sajssdk_2015_cross_new_user=1; __yadk_uid=UqbVJ5FY9B6HNOgxEwwbJ1CS8mNLpJ6G' \\\r\n\\
-H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' \\\r\n\\
-H 'Accept-Encoding: gzip, deflate, br' \\\r\n\\
-H 'Host: www.jianshu.com' \\\r\n\\
-H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.2 Safari/605.1.15' \\
-H 'Accept-Language: zh-cn' \\\r\n\\
-H 'Referer: https://www.baidu.com/link?url=f341HzRjG3imuqCPpHd-BmAeKmP_lMtx46n-B79TTWG2av7pJAA2-boDIILM8IX9&wd=&eqid=d36fd543002bccdc000000046044e97d' \\\r\n\\
-H 'Connection: keep-alive'",
    },
};

int main()
{
    spdlog::set_level(spdlog::level::debug);
    
    wish::http::message http_req(HTTP_BOTH);
    for (auto &cast : test_case)
    {
        http_req.parse_from_array(cast.request.c_str(), cast.request.size());
        http_req.parse_from_array(cast.response.c_str(), cast.response.size());
    }

}