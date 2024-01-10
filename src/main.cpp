#include <iostream>
#include <lua.hpp>  // bongo lua
#include <sfml.hpp> // sfml lua bindings

#include <global.hpp>
#include <helper.hpp>
#include <input.hpp>
#include <sprite.hpp>
#include <window.hpp>

sf::RenderWindow mainWindow;
sf::RenderStates mainRenderStates; // may cause artifacts when switching cats
lua_State *LuaState;

int main() {
    BongoLua::initializeLua();

    BongoInput::init();

    BongoSfml::bindToLua();
    BongoWindow::bindToLua();
    BongoSprite::bindToLua();
    BongoInput::bindToLua();
    BongoHelper::bindToLua();

    BongoLua::executeScript("src/test.lua");
    BongoLua::closeLua();

    return 0;
}
