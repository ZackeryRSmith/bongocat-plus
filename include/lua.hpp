#ifndef BONGO_CAT_LUA_HPP
#define BONGO_CAT_LUA_HPP
#include <global.hpp>

namespace BongoLua {
inline void initializeLua() {
    LuaState = luaL_newstate();
    luaL_openlibs(LuaState);
}

// execute cat script
inline bool executeScript(const std::string &filePath) {
    if (luaL_dofile(LuaState, filePath.c_str()) != LUA_OK) {
        std::cerr << "Failed to execute Lua script: "
                  << lua_tostring(LuaState, -1) << std::endl;
        return false;
    }
    return true;
}

// primarly for testing
inline bool executeCode(const std::string &code) {
    if (luaL_dostring(LuaState, code.c_str()) != LUA_OK) {
        std::cerr << "Failed to execute Lua code: "
                  << lua_tostring(LuaState, -1) << std::endl;
        return false;
    }
    return true;
}

inline void closeLua() { lua_close(LuaState); }
} // namespace BongoLua

#endif
