#include <global.hpp>
#include <input.hpp>
#include <sprite.hpp>
#include <window.hpp>

sf::Vector2i grabbed_offset;
bool grabbed_window = false;

unsigned int rstate_shift_width = 0;
unsigned int rstate_shift_height = 0;

//============================================================================
// CREATE
//============================================================================
void BongoWindow::create(UIntRef width, UIntRef height) {
    BongoWindow::create(width, height, sf::Style::Titlebar | sf::Style::Close);
}

void BongoWindow::create(UIntRef width, UIntRef height, UInt32Ref style) {
    mainWindow.create(sf::VideoMode(width, height), "BongoCat+", style);
    // check if the window being created is bigger then the screen
    if (mainWindow.getSize().x < width || mainWindow.getSize().y < height)
        std::cerr << "BongoCat+ [WARN]: Cannot spawn window of size (" << width
                  << ", " << height << "), window has been sized down to ("
                  << mainWindow.getSize().x << ", " << mainWindow.getSize().y
                  << ")";
    mainWindow.setFramerateLimit(MAX_FRAMERATE);
}

void BongoWindow::create(UIntRef width, UIntRef height, UInt32Ref style,
                         Vec2iRef position) {
    BongoWindow::create(width, height, style);
    mainWindow.setPosition(position);
}

//============================================================================
// GET WINDOW SIZE
//============================================================================
unsigned int BongoWindow::getWidth() { return mainWindow.getSize().x; }
unsigned int BongoWindow::getHeight() { return mainWindow.getSize().y; }
std::array<unsigned int, 2> BongoWindow::getSize() {
    return {mainWindow.getSize().x, mainWindow.getSize().y};
}

//============================================================================
// REFRESH
//============================================================================
void BongoWindow::refresh() {
    BongoWindow::create(mainWindow.getSize().x, mainWindow.getSize().y);
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
// TODO: this function is slow and consuming. Please fix this, it's called a lot
void BongoWindow::draw(const sf::Sprite &sprite) {
    const sf::FloatRect sprite_rect = sprite.getLocalBounds();

    if (sprite_rect.height > rstate_shift_height)
        rstate_shift_height = sprite_rect.height;

    sf::Transform transform = sf::Transform();
    transform.translate(0, mainWindow.getSize().y - BASE_HEIGHT);
    sf::RenderStates rstates = sf::RenderStates(transform);

    mainWindow.draw(sprite, mainRenderStates);
}

void BongoWindow::draw(const sf::Drawable &drawable) {
    mainWindow.draw(drawable, mainRenderStates);
}

void BongoWindow::draw(const sf::Vertex *vertices, size_t vertex_count,
                       sf::PrimitiveType type) {
    mainWindow.draw(vertices, vertex_count, type, mainRenderStates);
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
void BongoWindow::display() { mainWindow.display(); }

//============================================================================
// BIND TO LUA
//============================================================================
void BongoWindow::bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        .beginNamespace("BongoWindow")
        // create functions
        .addFunction(
            "create",
            luabridge::overload<UIntRef, UIntRef>(&BongoWindow::create),
            luabridge::overload<UIntRef, UIntRef, UInt32Ref>(
                &BongoWindow::create),
            luabridge::overload<UIntRef, UIntRef, UInt32Ref, Vec2iRef>(
                &BongoWindow::create))
        // get window size functions
        .addFunction("get_width", &BongoWindow::getWidth)
        .addFunction("get_height", &BongoWindow::getHeight)
        .addFunction("get_size", &BongoWindow::getSize)
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
