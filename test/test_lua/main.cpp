#include "core/luafunction/luafunction.h"
#include <iostream>
int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    nlohmann::json test;
    test["name"] = "wyw";
    test["age"] = 18;
    test["period"] = 11.11;
    test["array"] = nlohmann::json::value_t::array;
    test["array"].push_back("test1");
    test["array"].push_back("test2");
    test["array"].push_back("test3");
    nlohmann::json arr2 = nlohmann::json::value_t::array;
    test["array"].push_back(arr2);
    test["array"].push_back("test4");
    nlohmann::json obj3 = nlohmann::json::value_t::object;
    test["array"].push_back(obj3);
    test["array"].push_back("test7");

    
    test["object"] = {
        {"dream", "fly"},
        {"help", "....."},
        {"score", 100},
        {"null", nlohmann::json::value_t::null}
    };
    test["object2"] = nlohmann::json::value_t::object;


    wish::push_json(L, test);
    nlohmann::json result;
    wish::pop_json(L, result, -1);
    lua_pop(L, 1);
    for (auto& v : result.items())
    {
        std::cout << v.key() << " : " << v.value() << std::endl;
    }
    std::cout << "------------------" << std::endl;
    result.clear();

    luaL_dofile(L, "1.lua");

    int x = lua_gettop(L);

    lua_getglobal(L, "lua_test");
    lua_pushstring(L, "123");
    std::cout << lua_pcall(L, 1, 2, NULL) << std::endl;

    printf("%d %d %d %d\n", x, lua_type(L, -1), lua_type(L, -2), lua_gettop(L));
}