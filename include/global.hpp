#ifndef BONGO_CAT_GLOBAL_HPP
#define BONGO_CAT_GLOBAL_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
#define WINDOWS
#include <windows.h>
#elif defined(__APPLE__) || defined(__MACH__)
#define APPLE
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#elif defined(unix) || defined(__unix__) || defined(__unix) ||                 \
    defined(__linux__)
#define LINUX
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/keysym.h>

extern "C" {
#include <xdo.h>
}
#else
#define UNKNOWN
#endif

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include <LuaBridge/Array.h>
#include <LuaBridge/LuaBridge.h>

// used for std::clamp in input.cpp
#include <algorithm>

// types
using UIntRef = const unsigned int &;
using UInt32Ref = const uint32_t &;
using Vec2iRef = const sf::Vector2i &;

#define MAX_FRAMERATE 60
#define BASE_WIDTH 612
#define BASE_HEIGHT 354

// these symbols are defined in main.cpp
extern sf::RenderWindow main_window;
extern sf::RenderStates main_render_states;
extern lua_State *LuaState;

#endif
