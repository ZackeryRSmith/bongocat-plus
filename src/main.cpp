#include <iostream>

#include <global.hpp>
#include <lua.hpp>  // bongo lua
#include <sfml.hpp> // sfml lua bindings
#include <window.hpp>

sf::RenderWindow mainWindow;
lua_State *LuaState;

int main() {
    BongoLua::initializeLua();

    BongoWindow::bindToLua();
    BongoSfml::bindToLua();

    BongoLua::executeScript("src/test.lua");
    BongoLua::closeLua();

    return 0;
}
