#include <iostream>
#include <lua.hpp>  // bongo lua
#include <sfml.hpp> // sfml lua bindings

#include <audio.hpp>
#include <global.hpp>
#include <helper.hpp>
#include <input.hpp>
#include <sprite.hpp>
#include <window.hpp>

sf::RenderWindow mainWindow;
sf::RenderStates mainRenderStates; // may cause artifacts when switching cats
lua_State *LuaState;

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    FreeConsole(); // maybe not the best option as the console is still running
                   // with the console subsystem, but it does hide the console
#else
int main() {
#endif
    BongoLua::initializeLua();

    BongoInput::init();

    BongoSfml::bindToLua();
    BongoWindow::bindToLua();
    BongoSprite::bindToLua();
    BongoInput::bindToLua();
    BongoAudio::bindToLua();
    BongoHelper::bindToLua();

    BongoLua::executeScript("src/test.lua");
    BongoLua::closeLua();

    return 0;
}
