
/**************************************************************

                    BongoCat Api for LuaC

**************************************************************/

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <LuaBridge.h>
#include <iostream>

void sayHello(const std::string &name) {
    std::cout << "Hello, " << name << "!" << std::endl;
}

void boc_testing() {
    lua_State *L = luaL_newstate(); // create a Lua state
    luaL_openlibs(L);               // open the standard Lua libraries

    // Bind the sayHello function to the global namespace in Lua
    luabridge::getGlobalNamespace(L).addFunction("sayHello", sayHello);

    // A Lua script that calls the bound function
    const char *luaScript = R"(
        sayHello("World")
    )";

    // Execute the Lua script
    if (luaL_dostring(L, luaScript)) {
        std::cerr << lua_tostring(L, -1)
                  << std::endl; // If there's an error, print it
        lua_pop(L, 1);          // pop the error message from the stack
    }

    lua_close(L); // close the Lua state
}
