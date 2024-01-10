#ifndef BONGO_CAT_INPUT_HPP
#define BONGO_CAT_INPUT_HPP
#include <global.hpp>

#define TOTAl_INPUT_TABLE_SIZE 256
extern int INPUT_KEY_TABLE[TOTAl_INPUT_TABLE_SIZE];

// used in on_pressed & on_released
extern std::map<int, bool> keyState;

sf::Keyboard::Key ascii_to_key(int key_code);
bool is_pressed_fallback(int key_code);

namespace BongoInput {
//============================================================================
// INIT
//============================================================================
void init();
//============================================================================
// IS PRESSED
//============================================================================
bool isPressed(int key_code);
bool isPressed(char c);
//============================================================================
// ON PRESSED
//============================================================================
bool onPressed(int key_code);
bool onPressed(char c);
//============================================================================
// ON RELEASED
//============================================================================
bool onReleased(int key_code);
bool onReleased(char c);
//============================================================================
// MOUSE
//============================================================================
namespace Mouse {
//----------------------------------------------------------------------------
// positionOnFocusedWindow
//----------------------------------------------------------------------------
// get the position of the mouse relative to the current focused window's
std::array<double, 2> positionOnFocusedWindow();

//----------------------------------------------------------------------------
// positionOnHoveredWindow
//----------------------------------------------------------------------------
// get the position of the mouse relative to the window under the mouse
std::array<double, 2> positionOnHoveredWindow();

//----------------------------------------------------------------------------
// positionOnScreen
//----------------------------------------------------------------------------
// get the position of the mouse on the screen
std::array<double, 2> positionOnScreen();

}; // namespace Mouse
//============================================================================
//  BIND TO LUA
//============================================================================
void bindToLua();
} // namespace BongoInput
#endif
