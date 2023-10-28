#ifndef BONGO_CAT_SPRITE_HPP
#define BONGO_CAT_SPRITE_HPP
#include <global.hpp>

extern float max_sprite_width;
extern float max_sprite_height;

namespace BongoSprite {
//============================================================================
// LOAD FROM ...
//============================================================================
sf::Sprite *loadFromFile(const std::string &);
//============================================================================
// BIND TO LUA
//============================================================================
void bindToLua();
}; // namespace BongoSprite
#endif
