#ifndef BONGO_CAT_SFML_HPP
#define BONGO_CAT_SFML_HPP
#include <global.hpp>

/*
 * To future me...
 *
 * Please remove (or comment out) all SFML classes which SHOULDN'T be exposed to
 * the Lua api.
 *
 * For example, sf::RenderWindow shouldn't be touched at all, and should instead
 * be interfaced through BongoWindow. This avoids several issues, and forces the
 * user to use the one provided mainWindow
 *
 * If something needs to be accessed use an interface, like we do with
 * BongoWindow.
 */

namespace BongoSfml {
inline void bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        .beginNamespace("Sfml")
        //-------------------------------------------------------------------
        // sf::Vector2i
        //-------------------------------------------------------------------
        .beginClass<sf::Vector2i>("Vector2i")
        .addConstructor<void(), void(int, int)>()
        .addProperty("x", &sf::Vector2i::x)
        .addProperty("y", &sf::Vector2i::y)
        .endClass() // sf::Vector2i
        //-------------------------------------------------------------------
        // sf::Vector2f
        //-------------------------------------------------------------------
        .beginClass<sf::Vector2f>("Vector2f")
        .addConstructor<void(), void(float, float)>()
        .addProperty("x", &sf::Vector2f::x)
        .addProperty("y", &sf::Vector2f::y)
        .endClass() // sf::Vector2f
        //-------------------------------------------------------------------
        // sf::Vector2u
        //-------------------------------------------------------------------
        .beginClass<sf::Vector2u>("Vector2u")
        .addConstructor<void(), void(unsigned int, unsigned int)>()
        .addProperty("x", &sf::Vector2u::x)
        .addProperty("y", &sf::Vector2u::y)
        .endClass() // sf::Vector2u
        //-------------------------------------------------------------------
        // sf::IntRect
        //-------------------------------------------------------------------
        .beginClass<sf::IntRect>("IntRect")
        .addConstructor<void(), void(int, int, int, int)>()
        .addProperty("left", &sf::IntRect::left)
        .addProperty("top", &sf::IntRect::top)
        .addProperty("width", &sf::IntRect::width)
        .addProperty("height", &sf::IntRect::height)
        .addFunction(
            "contains", luabridge::overload<int, int>(&sf::IntRect::contains),
            luabridge::overload<const sf::Vector2i &>(&sf::IntRect::contains))
        //.addFunction(
        //    "intersects",
        //    static_cast<bool (sf::IntRect::*)(const sf::IntRect &) const>(
        //        &sf::IntRect::intersects))
        // missing intersects overload
        .addFunction("getPosition", &sf::IntRect::getPosition)
        .addFunction("getSize", &sf::IntRect::getSize)
        .endClass() // sf::IntRect
        //-------------------------------------------------------------------
        // sf::FloatRect
        //-------------------------------------------------------------------
        .beginClass<sf::FloatRect>("FloatRect")
        .addConstructor<void(), void(float, float, float, float)>()
        .addProperty("left", &sf::FloatRect::left)
        .addProperty("top", &sf::FloatRect::top)
        .addProperty("width", &sf::FloatRect::width)
        .addProperty("height", &sf::FloatRect::height)
        .addFunction(
            "contains",
            luabridge::overload<float, float>(&sf::FloatRect::contains),
            luabridge::overload<const sf::Vector2f &>(&sf::FloatRect::contains))

        //.addFunction(
        //    "intersects",
        //    static_cast<bool (sf::FloatRect::*)(const sf::FloatRect &) const>(
        //        &sf::FloatRect::intersects))
        // missing intersects overload
        .addFunction("getPosition", &sf::FloatRect::getPosition)
        .addFunction("getSize", &sf::FloatRect::getSize)
        .endClass() // sf::FloatRect
        //-------------------------------------------------------------------
        // sf::Color
        //-------------------------------------------------------------------
        .beginClass<sf::Color>("Color")
        .addConstructor<void(),
                        void(sf::Uint8, sf::Uint8, sf::Uint8, sf::Uint8),
                        void(sf::Uint32)>()
        .addProperty("r", &sf::Color::r)
        .addProperty("g", &sf::Color::g)
        .addProperty("b", &sf::Color::b)
        .addProperty("a", &sf::Color::a)
        .addFunction("toInteger", &sf::Color::toInteger)
        .addStaticProperty("Black", &sf::Color::Black)
        .addStaticProperty("White", &sf::Color::White)
        .addStaticProperty("Red", &sf::Color::Red)
        .addStaticProperty("Green", &sf::Color::Green)
        .addStaticProperty("Blue", &sf::Color::Blue)
        .addStaticProperty("Yellow", &sf::Color::Yellow)
        .addStaticProperty("Magenta", &sf::Color::Magenta)
        .addStaticProperty("Cyan", &sf::Color::Cyan)
        .addStaticProperty("Transparent", &sf::Color::Transparent)
        .endClass() // sf::Color
        //-------------------------------------------------------------------
        // sf::Transform == NOT FINISHED!
        //-------------------------------------------------------------------
        .beginClass<sf::Transform>("Transform")
        .addConstructor<void(), void(float, float, float, float, float, float,
                                     float, float, float)>()
        // continue to add everything
        .endClass() // sf::Transform
        //-------------------------------------------------------------------
        // sf::Image
        //-------------------------------------------------------------------
        .beginClass<sf::Image>("Image")
        .addConstructor<void()>()
        .addFunction(
            "create",
            luabridge::overload<unsigned int, unsigned int, const sf::Color &>(
                &sf::Image::create))
        .addFunction("loadFromFile", &sf::Image::loadFromFile)
        .addFunction("loadFromMemory", &sf::Image::loadFromMemory)
        // * loadFromStream ~ unimplemented
        .addFunction("getSize", &sf::Image::getSize)
        .addFunction("createMaskFromColor", &sf::Image::createMaskFromColor)
        .addFunction("copy", &sf::Image::copy)
        .addFunction("setPixel", &sf::Image::setPixel)
        .addFunction("getPixel", &sf::Image::getPixel)
        .addFunction("getPixelsPtr", &sf::Image::getPixelsPtr)
        .addFunction("flipHorizontally", &sf::Image::flipHorizontally)
        .addFunction("flipVertically", &sf::Image::flipVertically)
        .endClass() // sf::Image
        //-------------------------------------------------------------------
        // sf::Texture == NOT FINISHED
        //-------------------------------------------------------------------
        .beginClass<sf::Texture>("Texture")
        .addConstructor<void(), void(const sf::Texture &)>()
        .addFunction("create", &sf::Texture::create)
        .addFunction("loadFromFile", &sf::Texture::loadFromFile)
        .addFunction("loadFromMemory", &sf::Texture::loadFromMemory)
        // * loadFromStream ~ unimplemented
        // * loadFromImage ~ unimplemented
        .addFunction("getSize", &sf::Texture::getSize)
        // * copyToImage ~ unimplemented
        .addFunction("getSize", &sf::Texture::getSize)

        .endClass() // sf::Texture
        //-------------------------------------------------------------------
        // sf::Drawable
        //-------------------------------------------------------------------
        .beginClass<sf::Drawable>("Drawable")
        .endClass() // sf::Drawable
        //-------------------------------------------------------------------
        // sf::Sprite
        //-------------------------------------------------------------------
        //.deriveClass<sf::Sprite, sf::Drawable>("Sprite")
        .beginClass<sf::Sprite>("Sprite")
        .addConstructor<void(), void(const sf::Texture &),
                        void(const sf::Texture &, const sf::IntRect &)>()
        .addFunction("setTexture", &sf::Sprite::setTexture)
        .addFunction("setTextureRect", &sf::Sprite::setTextureRect)
        .addFunction("setColor", &sf::Sprite::setColor)
        .addFunction("getTexture", &sf::Sprite::getTexture)
        .addFunction("getColor", &sf::Sprite::getColor)
        .addFunction("getLocalBounds", &sf::Sprite::getLocalBounds)
        .addFunction("getGlobalBounds", &sf::Sprite::getGlobalBounds)
        .addFunction(
            "setPosition",
            luabridge::overload<float, float>(&sf::Sprite::setPosition),
            luabridge::overload<const sf::Vector2f &>(&sf::Sprite::setPosition))
        .addFunction("setRotation", &sf::Sprite::setRotation)
        .addFunction(
            "setScale",
            luabridge::overload<float, float>(&sf::Sprite::setScale),
            luabridge::overload<const sf::Vector2f &>(&sf::Sprite::setScale))
        .addFunction(
            "setOrigin",
            luabridge::overload<float, float>(&sf::Sprite::setOrigin),
            luabridge::overload<const sf::Vector2f &>(&sf::Sprite::setOrigin))
        .addFunction("getPosition", &sf::Sprite::getPosition)
        .addFunction("getRotation", &sf::Sprite::getRotation)
        .addFunction("getScale", &sf::Sprite::getScale)
        .addFunction("getOrigin", &sf::Sprite::getOrigin)
        .addFunction(
            "move", luabridge::overload<float, float>(&sf::Sprite::move),
            luabridge::overload<const sf::Vector2f &>(&sf::Sprite::move))
        .addFunction("rotate", &sf::Sprite::rotate)
        .addFunction(
            "scale", luabridge::overload<float, float>(&sf::Sprite::scale),
            luabridge::overload<const sf::Vector2f &>(&sf::Sprite::scale))
        .addFunction("getTransform", &sf::Sprite::getTransform)
        .addFunction("getInverseTransform", &sf::Sprite::getInverseTransform)
        .endClass() // sf::Sprite
        //-------------------------------------------------------------------
        // sf::Style
        //-------------------------------------------------------------------
        .beginNamespace("Style")
        .addVariable("None", sf::Style::None)
        .addVariable("Titlebar", sf::Style::Titlebar)
        .addVariable("Resize", sf::Style::Resize)
        .addVariable("Close", sf::Style::Close)
        .addVariable("Fullscreen", sf::Style::Fullscreen)
        .addVariable("Default", sf::Style::Default)
        .endNamespace() // sf::Style
        //-------------------------------------------------------------------
        // sf::VideoMode
        //-------------------------------------------------------------------
        .beginClass<sf::VideoMode>("VideoMode")
        .endClass() // sf::VideoMode
        //-------------------------------------------------------------------
        // sf::ContextSettings
        //-------------------------------------------------------------------
        .beginClass<sf::ContextSettings>("ContextSettings")
        .endClass() // sf::ContextSettings
        //-------------------------------------------------------------------
        // sf::String
        //-------------------------------------------------------------------
        .beginClass<sf::String>("String")
        .endClass() // sf::String

        .endNamespace(); // Sfml
}
}; // namespace BongoSfml

#endif
