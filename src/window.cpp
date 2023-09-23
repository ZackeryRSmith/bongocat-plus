#include <global.hpp>
#include <window.hpp>

sf::Vector2i grabbed_offset;
bool grabbed_window = false;

//============================================================================
// CREATE
//============================================================================
void BongoWindow::create(UIntRef width, UIntRef height, UInt32Ref style) {
    mainWindow.create(sf::VideoMode(width, height), "BongoCat+", style);
    mainWindow.setFramerateLimit(MAX_FRAMERATE);
}

void BongoWindow::create(UIntRef width, UIntRef height, UInt32Ref style,
                         Vec2iRef position) {
    BongoWindow::create(width, height, style);
    mainWindow.setPosition(position);
}

//============================================================================
// REFRESH
//============================================================================
void BongoWindow::refresh() {
    BongoWindow::create(mainWindow.getSize().x, mainWindow.getSize().y,
                        sf::Style::Default);
}

void BongoWindow::refresh(UIntRef width, UIntRef height, UInt32Ref style) {
    BongoWindow::create(width, height, style);
}

void BongoWindow::refresh(UIntRef width, UIntRef height, UInt32Ref style,
                          Vec2iRef position) {
    BongoWindow::create(width, height, style, position);
}

//============================================================================
// PROCESS EVENTS
//============================================================================
int BongoWindow::processEvents() {
    sf::Event event;
    while (mainWindow.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            BongoWindow::close();
            return 1;

        case sf::Event::KeyPressed:
            // reload window
            if (event.key.code == sf::Keyboard::R && event.key.control) {
                BongoWindow::refresh();
                break;
            }

            // toggle debug panel
            if (event.key.code == sf::Keyboard::D && event.key.control) {
                // is_show_input_debug = !is_show_input_debug;
                break;
            }

        // for mice this system works great, for trackpads however not so
        // much... As trackpads have acceleration the faster you move you can
        // drag your mouse off the window.
        case sf::Event::MouseButtonPressed:
            // if (event.mouseButton.button == sf::Mouse::Left && borderless) {
            // grabbed_offset =
            //     window.getPosition() - sf::Mouse::getPosition();
            // grabbed_window = true;
            //}
            break;

        case sf::Event::MouseButtonReleased:
            // if (event.mouseButton.button == sf::Mouse::Left && borderless)
            //     grabbed_window = false;
            break;

        case sf::Event::MouseMoved:
            // if (grabbed_window && borderless)
            //     window.setPosition(sf::Mouse::getPosition() +
            //     grabbed_offset);
            break;

        default:
            break;
        }
    }

    return 0;
}

//============================================================================
// CLOSE
//============================================================================
void BongoWindow::close() { mainWindow.close(); }

//============================================================================
// CLEAR
//============================================================================
void BongoWindow::clear() { mainWindow.clear(); }
void BongoWindow::clear(const sf::Color &color) { mainWindow.clear(color); }

//============================================================================
// DRAW
//============================================================================
void BongoWindow::draw(const sf::Drawable &drawable) {
    mainWindow.draw(drawable);
}

//============================================================================
// DISPLAY
//============================================================================
void BongoWindow::display() { mainWindow.display(); }

//============================================================================
// BIND TO LUA
//============================================================================
// no need to ever bind BongoWindow to lua, but here if you must
void BongoWindow::bindToLua() {
    void (*createFunc1)(UIntRef, UIntRef, UInt32Ref) = BongoWindow::create;
    void (*createFunc2)(UIntRef, UIntRef, UInt32Ref, Vec2iRef) =
        BongoWindow::create;

    void (*refreshFunc1)() = BongoWindow::refresh;
    void (*refreshFunc2)(UIntRef, UIntRef, UInt32Ref) = BongoWindow::refresh;
    void (*refreshFunc3)(UIntRef, UIntRef, UInt32Ref, Vec2iRef) =
        BongoWindow::refresh;

    luabridge::getGlobalNamespace(LuaState)
        .beginNamespace("BongoWindow")
        // create functions
        .addFunction("create", createFunc1)
        .addFunction("createWithPosition", createFunc2)
        // refresh functions
        .addFunction("refresh", refreshFunc1)
        .addFunction("refreshWithSizeAndStyle", refreshFunc2)
        .addFunction("refreshWithPosition", refreshFunc3)
        // rendering functions
        .addFunction(
            "clear", luabridge::overload<>(&BongoWindow::clear),
            luabridge::overload<const sf::Color &>(&BongoWindow::clear))
        .addFunction("draw", &BongoWindow::draw)
        .addFunction("display", &BongoWindow::display)
        // other functions
        .addFunction("processEvents", &BongoWindow::processEvents)
        .addFunction("close", &BongoWindow::close)
        .endNamespace();
}
