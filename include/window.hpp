#ifndef BONGO_CAT_WINDOW_HPP
#define BONGO_CAT_WINDOW_HPP
#include <global.hpp>

// should really be moved into the namespace
//============================================================================
// PREPROCESSORS
//============================================================================
extern sf::Vector2i grabbed_offset;
extern bool grabbed_window;

// stores how much to shift the render state
extern unsigned int rstate_shift_width;
extern unsigned int rstate_shift_height;

extern float globalScale;

namespace BongoWindow {
//============================================================================
// CREATE
//============================================================================
void create(UIntRef width, UIntRef height);
void create(UIntRef width, UIntRef height, UInt32Ref style);
void create(UIntRef width, UIntRef height, UInt32Ref style, Vec2iRef position);

//============================================================================
// GET WINDOW SIZE
//============================================================================
unsigned int getWidth();
unsigned int getHeight();
std::array<unsigned int, 2> getSize();

//============================================================================
// REFRESH
//============================================================================
void refresh();
void refresh(UIntRef width, UIntRef height, UInt32Ref style);
void refresh(UIntRef width, UIntRef height, UInt32Ref style, Vec2iRef position);

//============================================================================
// PROCESS EVENTS
//============================================================================
int processEvents();

//============================================================================
// CLEAR
//============================================================================
void clear();
void clear(const sf::Color &color);

//============================================================================
// DRAW
//============================================================================
void draw(sf::Sprite &sprite);
void draw(const sf::Drawable &drawable);

//============================================================================
// DRAW IF
//============================================================================
void drawif(sf::Sprite &sprite, bool condition);
void drawif(const sf::Drawable &drawable, bool condition);

//============================================================================
// DISPLAY
//============================================================================
void display();

//============================================================================
// CLOSE
//============================================================================
void close();

//============================================================================
// BIND TO LUA
//============================================================================
void bindToLua();
} // namespace BongoWindow

#endif
