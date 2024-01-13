#include <global.hpp>
#include <sprite.hpp>

float max_sprite_width = 0.0;
float max_sprite_height = 0.0;

//============================================================================
// LOAD FROM ...
//============================================================================
sf::Sprite *BongoSprite::loadFromFile(const std::string &path) {
    sf::Texture *texture = new sf::Texture();
    texture->loadFromFile(path);

    sf::Sprite *sprite = new sf::Sprite();
    sprite->setTexture(*texture);

    if (sprite->getLocalBounds().width > max_sprite_width)
        max_sprite_width = sprite->getLocalBounds().width;
    if (sprite->getLocalBounds().height > max_sprite_height)
        max_sprite_height = sprite->getLocalBounds().height;

    // std::cout << "max sw: " << max_sprite_width
    //           << "  max sh: " << max_sprite_height << std::endl;

    return sprite;
}

//============================================================================
// BIND TO LUA
//============================================================================
void BongoSprite::bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        .beginNamespace("BongoSprite")
        // load functions
        .addFunction("loadFromFile", &BongoSprite::loadFromFile)
        .endNamespace();
}
