#include <global.hpp>
#include <input.hpp>
#include <sprite.hpp>
#include <window.hpp>

// TODO: I really need to go about making functions start returning sf::Vector2... stuff.
//       Seriously, although returning an array is nice and all for lua binding it annoys
//       me when trying to use those functions with SFML code which requires a sf::Vector2...

// borderless window support
sf::Vector2i grabbed_offset;
bool grabbed_window = false;
bool borderless = false;

//============================================================================
// CREATE
//============================================================================
void BongoWindow::create() {
    BongoWindow::create(max_sprite_width, max_sprite_height);
}

void BongoWindow::create(UInt32Ref style) {
    BongoWindow::create(max_sprite_width, max_sprite_height, style);
}

void BongoWindow::create(UIntRef width, UIntRef height) {
    BongoWindow::create(width, height, sf::Style::Titlebar | sf::Style::Close);
}

// NOTE: All BongoWindow create functions rely on this function
void BongoWindow::create(UIntRef width, UIntRef height, UInt32Ref style) {
    if (style == sf::Style::None)
        borderless = true;

    main_window.create(sf::VideoMode(width, height), "BongoCat+", style);
    // check if the window being created is bigger then the screen
    if (BongoWindow::getX() < width || BongoWindow::getY() < height)
        std::cerr << "BongoCat+ [WARN]: Cannot spawn window of size (" << width
                  << ", " << height << "), window has been sized down to ("
                  << BongoWindow::getX() << ", " << BongoWindow::getY()
                  << ")";
    main_window.setFramerateLimit(MAX_FRAMERATE);
}

void BongoWindow::create(UIntRef width, UIntRef height, UInt32Ref style,
                         Vec2iRef position) {
    BongoWindow::create(width, height, style);
    BongoWindow::setPosition(position);
}

//============================================================================
// GET WINDOW SIZE
//============================================================================
unsigned int BongoWindow::getWidth() { return main_window.getSize().x; }
unsigned int BongoWindow::getHeight() { return main_window.getSize().y; }
std::array<unsigned int, 2> BongoWindow::getSize() {
    return {main_window.getSize().x, main_window.getSize().y};
}

//============================================================================
// GET WINDOW POSITION
//============================================================================
unsigned int BongoWindow::getX() { return main_window.getSize().x; }
unsigned int BongoWindow::getY() { return main_window.getSize().y; }
std::array<int, 2> BongoWindow::getPosition() {
    const sf::Vector2i pos = main_window.getPosition();
    return {pos.x, pos.y};
}

//============================================================================
// SET WINDOW POSITION
//============================================================================
void BongoWindow::setX(int x) {
    BongoWindow::setPosition(sf::Vector2i(x, BongoWindow::getY()));
}
void BongoWindow::setY(int y) {
    BongoWindow::setPosition(sf::Vector2i(BongoWindow::getX(), y));
}
void BongoWindow::setPosition(sf::Vector2i pos) {
    main_window.setPosition(pos);
}

//============================================================================
// REFRESH
//============================================================================
void BongoWindow::refresh() {
    BongoWindow::create(main_window.getSize().x, main_window.getSize().y);
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
    while (main_window.pollEvent(event)) { // MAYBE: a good idea to allow lua to poll events
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
            if (event.mouseButton.button == sf::Mouse::Left && borderless) {
                auto [x, y] = BongoInput::Mouse::positionOnScreen();
                grabbed_offset = main_window.getPosition() - sf::Vector2i(x, y);
                grabbed_window = true;
            }
            break;

        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left && borderless)
                grabbed_window = false;
            break;

        case sf::Event::MouseMoved:
            if (grabbed_window && borderless) {
                auto [x, y] = BongoInput::Mouse::positionOnScreen();
                BongoWindow::setPosition(sf::Vector2i(x, y) + grabbed_offset);
            }

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
void BongoWindow::close() { main_window.close(); }

//============================================================================
// CLEAR
//============================================================================
void BongoWindow::clear() { main_window.clear(); }
void BongoWindow::clear(const sf::Color &color) { main_window.clear(color); }

//============================================================================
// DRAW
//============================================================================
// TODO: this function is slow and consuming. Please fix this, it's called a lot
void BongoWindow::draw(const sf::Sprite &sprite) {
    const sf::FloatRect sprite_rect = sprite.getLocalBounds();

    sf::Transform transform = sf::Transform();
    transform.translate(0, BongoWindow::getY() - BASE_HEIGHT);
    sf::RenderStates rstates = sf::RenderStates(transform);

    main_window.draw(sprite, main_render_states);
}

void BongoWindow::draw(const sf::Drawable &drawable) {
    main_window.draw(drawable, main_render_states);
}

void BongoWindow::draw(const sf::Vertex *vertices, size_t vertex_count,
                       sf::PrimitiveType type) {
    main_window.draw(vertices, vertex_count, type, main_render_states);
}

//============================================================================
// DRAW IF
//============================================================================
void BongoWindow::drawif(const sf::Sprite &sprite, bool condition) {
    if (condition)
        BongoWindow::draw(sprite);
}
void BongoWindow::drawif(const sf::Drawable &drawable, bool condition) {
    if (condition)
        BongoWindow::draw(drawable);
}

//============================================================================
// DRAW IF ELSE
//============================================================================
void BongoWindow::drawifelse(const sf::Sprite &true_sprite, bool condition,
                             const sf::Sprite &false_sprite) {
    if (condition)
        BongoWindow::draw(true_sprite);
    else
        BongoWindow::draw(false_sprite);
}
void BongoWindow::drawifelse(const sf::Drawable &true_drawable, bool condition,
                             const sf::Drawable &false_drawable) {
    if (condition)
        BongoWindow::draw(true_drawable);
    else
        BongoWindow::draw(false_drawable);
}

//============================================================================
// DISPLAY
//============================================================================
void BongoWindow::display() { main_window.display(); }

//============================================================================
// BIND TO LUA
//============================================================================
void BongoWindow::bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        .beginNamespace("BongoWindow")
        // create functions
        .addFunction(
            "create", luabridge::overload<>(&BongoWindow::create),
            luabridge::overload<UInt32Ref>(&BongoWindow::create),
            luabridge::overload<UIntRef, UIntRef>(&BongoWindow::create),
            luabridge::overload<UIntRef, UIntRef, UInt32Ref>(
                &BongoWindow::create),
            luabridge::overload<UIntRef, UIntRef, UInt32Ref, Vec2iRef>(
                &BongoWindow::create))
        // get window size functions
        .addFunction("getWidth", &BongoWindow::getWidth)
        .addFunction("getHeight", &BongoWindow::getHeight)
        .addFunction("getSize", &BongoWindow::getSize)
        // get window position functions
        .addFunction("getX", &BongoWindow::getX)
        .addFunction("getY", &BongoWindow::getY)
        .addFunction("getPosition", &BongoWindow::getPosition)
        // set window size functions
        // IMPL: please implement these soon
        //.addFunction("setWidth", &BongoWindow::setWidth)
        //.addFunction("setHeight", &BongoWindow::setHeight)
        //.addFunction("setSize", &BongoWindow::setSize) 
        // set window position functions
        .addFunction("setX", &BongoWindow::setX)
        .addFunction("setY", &BongoWindow::setY)
        .addFunction("setPosition", &BongoWindow::setPosition)
        // refresh functions
        .addFunction("refresh", luabridge::overload<>(&BongoWindow::refresh),
                     luabridge::overload<UIntRef, UIntRef, UInt32Ref>(
                         &BongoWindow::refresh),
                     luabridge::overload<UIntRef, UIntRef, UInt32Ref, Vec2iRef>(
                         &BongoWindow::refresh))
        // rendering functions
        .addFunction(
            "clear", luabridge::overload<>(&BongoWindow::clear),
            luabridge::overload<const sf::Color &>(&BongoWindow::clear))
        // drawing functions
        .addFunction(
            "draw", luabridge::overload<const sf::Sprite &>(&BongoWindow::draw),
            luabridge::overload<const sf::Drawable &>(&BongoWindow::draw))
        .addFunction(
            "drawif",
            luabridge::overload<const sf::Sprite &, bool>(&BongoWindow::drawif),
            luabridge::overload<const sf::Drawable &, bool>(
                &BongoWindow::drawif))
        .addFunction(
            "drawifelse",
            luabridge::overload<const sf::Sprite &, bool, const sf::Sprite &>(
                &BongoWindow::drawifelse),
            luabridge::overload<const sf::Drawable &, bool,
                                const sf::Drawable &>(&BongoWindow::drawifelse))
        .addFunction("display", &BongoWindow::display)
        // other functions
        .addFunction("processEvents", &BongoWindow::processEvents)
        .addFunction("close", &BongoWindow::close)
        .endNamespace();
}
