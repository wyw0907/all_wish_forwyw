#pragma once

#include <lua/lua.hpp>
#include <json/json.hpp>


namespace wish {
int push_json(lua_State*L, const nlohmann::json& obj);

int pop_json(lua_State *L, nlohmann::json& obj, int index = -1);
}