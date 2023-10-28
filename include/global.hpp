#ifndef BONGO_CAT_GLOBAL_HPP
#define BONGO_CAT_GLOBAL_HPP
extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <LuaBridge.h>
#include <SFML/Graphics.hpp>

// types
using UIntRef = const unsigned int &;
using UInt32Ref = const uint32_t &;
using Vec2iRef = const sf::Vector2i &;

#define MAX_FRAMERATE 60
#define BASE_WIDTH 612
#define BASE_HEIGHT 354

// these symbols are defined in main.cpp
extern sf::RenderWindow mainWindow;
extern sf::RenderStates mainRenderStates;
extern lua_State *LuaState;

#endif
