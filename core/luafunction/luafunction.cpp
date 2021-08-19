#include "luafunction.h"
using nlohmann::json;
#include <iostream>
namespace wish{
int push_json(lua_State*L, const json& obj)
{
    size_t count = 1;
    switch (obj.type())
    {
    case json::value_t::null :
        lua_pushnil(L);
        break;
    case json::value_t::boolean:
        lua_pushboolean(L, obj.get<bool>());
        break;
    case json::value_t::string:
        lua_pushstring(L, obj.get<std::string>().c_str());
        break;
    case json::value_t::number_unsigned:
        lua_pushinteger(L, obj.get<uint64_t>());
        break;
    case json::value_t::number_integer:
        lua_pushinteger(L, obj.get<int64_t>());
        break;
    case json::value_t::number_float:
        lua_pushnumber(L, obj.get<lua_Number>());
        break;
    case json::value_t::array:
        {
            count = 0;
            if (obj.size() > 0)
            {
                lua_createtable(L, obj.size(), 0);
            }
            for (size_t idx = 0; idx < obj.size(); ++idx)
            {
                if (push_json(L, obj[idx]) > 0)
                {
                    count++;
                    lua_rawseti(L, -2, idx + 1);
                }
            }
            break;
        }
    case json::value_t::object:
        {
            //count = 0;
            lua_createtable(L, 0, obj.size());
            for (auto& v : obj.items())
            {
                const auto& name = v.key();
                if (push_json(L, v.value()) > 0)
                {
                    count ++;
                    lua_setfield(L, -2, name.c_str());
                }
            }
            break;
        }
    default:
        count = 0;
        break;
    }
    return count;
}

int pop_json(lua_State *L, json& obj, int index)
{
    int type = lua_type(L, index);
    switch (type)
    {
    case LUA_TNIL:
        obj = json::value_t::null;
        break;
    case LUA_TBOOLEAN:
        obj = lua_toboolean(L, index);
        break;
    case LUA_TNUMBER:
        obj = luaL_checknumber(L, index);
        break;
    case LUA_TSTRING:
        obj = luaL_checkstring(L, index);
        break;
    case LUA_TTABLE:
        {
            lua_pushnil(L);
            int table_pos = index >= 0 ? index : index - 1;
            while( 0 != lua_next(L, table_pos))
            {
                json value;
                pop_json(L, value, -1);
                if (obj.is_null())
                {
                    int table_type = lua_type(L, -2);
                    switch (table_type)
                    {
                    case LUA_TNUMBER:
                        obj = json::value_t::array;
                        break;
                    case LUA_TSTRING:
                        obj = json::value_t::object;
                    default:
                        break;
                    }
                }

                if(obj.is_array())
                {
                    obj.push_back(std::move(value));
                }
                else if(obj.is_object())
                {
                    const char *key = luaL_checkstring(L, -2);
                    if (key)
                    {
                        obj[key] = std::move(value);
                    }
                }
                lua_pop(L, 1);
            }
        }
        break;
    default:
        obj = json::value_t::null;
        break;
    }
    return type;
}
}