#include <global.hpp>
#include <input.hpp>

int INPUT_KEY_TABLE[TOTAl_INPUT_TABLE_SIZE];

std::map<int, bool> keyState;

sf::Keyboard::Key ascii_to_key(int key_code) {
    if (key_code < 0 || key_code >= TOTAl_INPUT_TABLE_SIZE) {
        // out of range
        return sf::Keyboard::Unknown;
    } else {
        return (sf::Keyboard::Key)(INPUT_KEY_TABLE[key_code]);
    }
}

bool is_pressed_fallback(int key_code) {
#if defined(WINDOWS)
    return (GetAsyncKeyState(key_code) & 0x8000) != 0;
#elif defined(APPLE)
    // BUG: this is entirly broken
    UInt32 key_map[4] = {0};
    CGEventRef event =
        CGEventCreateKeyboardEvent(NULL, (CGKeyCode)key_code, true);
    CGEventFlags flags = CGEventGetFlags(event);
    CFRelease(event);
    return (flags & kCGEventFlagMaskCommand) != 0;
#elif defined(LINUX)
    KeyCode keycode = XKeysymToKeycode(dpy, key_code);
    if (keycode != 0) {
        char keys[32];
        XQueryKeymap(dpy, keys);
        return (keys[keycode / 8] & (1 << (keycode % 8))) != 0;
    } else {
        return false;
    }
#endif
}

//============================================================================
// INIT
//============================================================================
void BongoInput::init() {
    for (int i = 0; i < TOTAl_INPUT_TABLE_SIZE; i++) {
        if (i >= 48 && i <= 57) { // number
            INPUT_KEY_TABLE[i] = i - 48 + (int)sf::Keyboard::Key::Num0;
        } else if (i >= 65 && i <= 90) { // english alphabet
            INPUT_KEY_TABLE[i] = i - 65 + (int)sf::Keyboard::Key::A;
        } else if (i >= 96 && i <= 105) { // numpad
            INPUT_KEY_TABLE[i] = i - 96 + (int)sf::Keyboard::Key::Numpad0;
        } else if (i >= 112 && i <= 126) { // function
            INPUT_KEY_TABLE[i] = i - 112 + (int)sf::Keyboard::Key::F1;
        } else {
            INPUT_KEY_TABLE[i] = (int)sf::Keyboard::Key::Unknown;
        }
    }

    INPUT_KEY_TABLE[16] = (int)sf::Keyboard::Key::RShift;
    INPUT_KEY_TABLE[27] = (int)sf::Keyboard::Key::Escape;
    INPUT_KEY_TABLE[17] = (int)sf::Keyboard::Key::LControl;
    INPUT_KEY_TABLE[16] = (int)sf::Keyboard::Key::LShift;
    INPUT_KEY_TABLE[18] = (int)sf::Keyboard::Key::LAlt;
    INPUT_KEY_TABLE[17] = (int)sf::Keyboard::Key::RControl;
    INPUT_KEY_TABLE[18] = (int)sf::Keyboard::Key::RAlt;
    INPUT_KEY_TABLE[93] = (int)sf::Keyboard::Key::Menu;
    INPUT_KEY_TABLE[219] = (int)sf::Keyboard::Key::LBracket;
    INPUT_KEY_TABLE[221] = (int)sf::Keyboard::Key::RBracket;
    INPUT_KEY_TABLE[186] = (int)sf::Keyboard::Key::Semicolon;
    INPUT_KEY_TABLE[188] = (int)sf::Keyboard::Key::Comma;
    INPUT_KEY_TABLE[190] = (int)sf::Keyboard::Key::Period;
    INPUT_KEY_TABLE[222] = (int)sf::Keyboard::Key::Quote;
    INPUT_KEY_TABLE[191] = (int)sf::Keyboard::Key::Slash;
    INPUT_KEY_TABLE[220] = (int)sf::Keyboard::Key::Backslash;
    INPUT_KEY_TABLE[192] = (int)sf::Keyboard::Key::Tilde;
    INPUT_KEY_TABLE[187] = (int)sf::Keyboard::Key::Equal;
    INPUT_KEY_TABLE[189] = (int)sf::Keyboard::Key::Hyphen;
    INPUT_KEY_TABLE[32] = (int)sf::Keyboard::Key::Space;
    INPUT_KEY_TABLE[13] = (int)sf::Keyboard::Key::Enter;
    INPUT_KEY_TABLE[8] = (int)sf::Keyboard::Key::Backspace;
    INPUT_KEY_TABLE[9] = (int)sf::Keyboard::Key::Tab;
    INPUT_KEY_TABLE[33] = (int)sf::Keyboard::Key::PageUp;
    INPUT_KEY_TABLE[34] = (int)sf::Keyboard::Key::PageDown;
    INPUT_KEY_TABLE[35] = (int)sf::Keyboard::Key::End;
    INPUT_KEY_TABLE[36] = (int)sf::Keyboard::Key::Home;
    INPUT_KEY_TABLE[45] = (int)sf::Keyboard::Key::Insert;
    INPUT_KEY_TABLE[46] = (int)sf::Keyboard::Key::Delete;
    INPUT_KEY_TABLE[107] = (int)sf::Keyboard::Key::Add;
    INPUT_KEY_TABLE[109] = (int)sf::Keyboard::Key::Subtract;
    INPUT_KEY_TABLE[106] = (int)sf::Keyboard::Key::Multiply;
    INPUT_KEY_TABLE[111] = (int)sf::Keyboard::Key::Divide;
    INPUT_KEY_TABLE[37] = (int)sf::Keyboard::Key::Left;
    INPUT_KEY_TABLE[39] = (int)sf::Keyboard::Key::Right;
    INPUT_KEY_TABLE[38] = (int)sf::Keyboard::Key::Up;
    INPUT_KEY_TABLE[40] = (int)sf::Keyboard::Key::Down;
    INPUT_KEY_TABLE[19] = (int)sf::Keyboard::Key::Pause;
    INPUT_KEY_TABLE[189] = (int)sf::Keyboard::Key::Dash;
}

//============================================================================
// IS PRESSED
//============================================================================
bool BongoInput::isPressed(int key_code) {
    if (key_code == 16) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    } else if (key_code == 17) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    } else {
        sf::Keyboard::Key selected = ascii_to_key(toupper(key_code));
        if (selected != sf::Keyboard::Key::Unknown) {
            return sf::Keyboard::isKeyPressed(selected);
        } else {
            return is_pressed_fallback(key_code);
        }
    }
}
bool BongoInput::isPressed(char c) { return BongoInput::isPressed((int)c); }

//============================================================================
// ON PRESSED
//============================================================================
bool BongoInput::onPressed(int key_code) {
    bool isCurrentlyPressed = BongoInput::isPressed(key_code);
    bool wasPressedLastFrame = keyState[key_code];

    keyState[key_code] = isCurrentlyPressed;

    // if the key is currently pressed and was not pressed last frame
    return isCurrentlyPressed && !wasPressedLastFrame;
}
bool BongoInput::onPressed(char c) { return BongoInput::onPressed((int)c); }

//============================================================================
// ON RELEASED
//============================================================================
bool BongoInput::onReleased(int key_code) {
    bool isCurrentlyPressed = BongoInput::isPressed(key_code);
    bool wasPressedLastFrame = keyState[key_code];

    keyState[key_code] = isCurrentlyPressed;

    // if the key was pressed last frame and is not currently pressed
    return wasPressedLastFrame && !isCurrentlyPressed;
}
bool BongoInput::onReleased(char c) { return BongoInput::onReleased((int)c); }

//============================================================================
// POSITION ON SCREEN
//============================================================================
std::array<double, 2> BongoInput::Mouse::positionOnScreen() {
    double x, y;

#if defined(WINDOWS)
    // TODO: impl.
#elif defined(APPLE)
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CFRelease(event);

    x = cursor.x;
    y = cursor.y;
#elif defined(LINUX)
    // TODO: impl.
#endif

    return {x, y};
}

//============================================================================
// POSITION ON FOCUSED WINDOW
//============================================================================
std::array<double, 2> BongoInput::Mouse::positionOnFocusedWindow() {
    double x, y;
    x = 0;
    y = 0;

#if defined(WINDOWS)
    // TODO: impl.
#elif defined(APPLE)
    // clang-format off
    // BUG:
    //  - when the terminal is running a subprocess it gets very buggy

    CFArrayRef windows = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);

    if (windows) {
        // abs_ is just relating to the position on the entire screen
        auto [abs_x, abs_y] = BongoInput::Mouse::positionOnScreen();
        
        CFIndex count = CFArrayGetCount(windows);

        if (count > 0) {
            CFDictionaryRef window_info = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, 0);
            CFDictionaryRef bounds_ref = (CFDictionaryRef)CFDictionaryGetValue(window_info, kCGWindowBounds);

            // NOTE: can error
            CGRect bounds = CGRect();
            CGRectMakeWithDictionaryRepresentation(bounds_ref, &bounds);

            // offset the cursor position within the window bounds
            // rel_ = relative
            double rel_x = abs_x - bounds.origin.x;
            double rel_y = abs_y - bounds.origin.y;

            x = std::clamp(rel_x, 0.0, bounds.size.width);
            y = std::clamp(rel_y, 0.0, bounds.size.height);
        }
    }

    CFRelease(windows);

    // clang-format on
#elif defined(LINUX)
    // TODO: impl.
#endif

    return {x, y};
}

//============================================================================
// POSITION ON HOVERED WINDOW
//============================================================================
std::array<double, 2> BongoInput::Mouse::positionOnHoveredWindow() {
    double x, y;
    x = 0;
    y = 0;

#if defined(WINDOWS)
    // TODO: impl.
#elif defined(APPLE)
    // clang-format off
    // BUG:
    //  - when the terminal is running a subprocess it gets very buggy
    //  - just overall buggy, issue with how bounds are grabbed

    CFArrayRef windows = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);

    if (windows) {
        // abs_ is just relating to the position on the entire screen
        auto [abs_x, abs_y] = BongoInput::Mouse::positionOnScreen();
        
        CFIndex count = CFArrayGetCount(windows);

        if (count > 0) {
            for (int i = 0; count >= i; i++) {
                CFDictionaryRef window_info = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, i);
                CFDictionaryRef bounds_ref = (CFDictionaryRef)CFDictionaryGetValue(window_info, kCGWindowBounds);
            
                // NOTE: can error
                CGRect bounds = CGRect();
                CGRectMakeWithDictionaryRepresentation(bounds_ref, &bounds);

                if (bounds.origin.x <= abs_x <= bounds.size.width && 
                    bounds.origin.y <= abs_y <= bounds.size.height) {

                    // offset the cursor position within the window bounds
                    // rel_ = relative
                    double rel_x = abs_x - bounds.origin.x;
                    double rel_y = abs_y - bounds.origin.y;
                
                    x = std::clamp(rel_x, 0.0, bounds.size.width);
                    y = std::clamp(rel_y, 0.0, bounds.size.height);
                    break;
                }
            }
        }

    }

    CFRelease(windows);

    // clang-format on
#elif defined(LINUX)
    // TODO: impl.
#endif

    return {x, y};
}

//============================================================================
// BIND TO LUA
//============================================================================
void BongoInput::bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        // TODO: move this else where
        .beginClass<std::pair<double, double>>("pair")
        .addConstructor<void (*)(const double &, const double &)>()
        .addProperty("first", &std::pair<double, double>::first)
        .addProperty("second", &std::pair<double, double>::second)
        .endClass()

        .addFunction("isPressed",
                     luabridge::overload<int>(&BongoInput::isPressed),
                     luabridge::overload<char>(&BongoInput::isPressed))
        .addFunction("onPressed",
                     luabridge::overload<int>(&BongoInput::onPressed),
                     luabridge::overload<char>(&BongoInput::onPressed))
        .addFunction("onReleased",
                     luabridge::overload<int>(&BongoInput::onReleased),
                     luabridge::overload<char>(&BongoInput::onReleased))
        .beginNamespace("BongoInput")
        // Mouse
        .beginNamespace("Mouse")
        .addFunction("positionOnScreen", &BongoInput::Mouse::positionOnScreen)
        .addFunction("positionOnFocusedWindow",
                     &BongoInput::Mouse::positionOnFocusedWindow)
        .addFunction("positionOnHoveredWindow",
                     &BongoInput::Mouse::positionOnHoveredWindow)
        .endNamespace()
        // Key
        .beginNamespace("Key")
        // INFO: based off http://www.foreui.com/articles/Key_Code_Table.htm
        // NOTE: 0 - 49
        .addVariable("backspace", 8)
        .addVariable("tab", 9)
        .addVariable("enter", 13)
        .addVariable("shift", 16)
        .addVariable("ctrl", 27)
        .addVariable("alt", 18)
        .addVariable("pause", 19)
        .addVariable("caps_lock", 20)
        .addVariable("escape", 27)
        .addVariable("page_up", 33)
        .addVariable("page_down", 34)
        .addVariable("end", 35)
        .addVariable("home", 36)
        .addVariable("left", 37)
        .addVariable("up", 38)
        .addVariable("right", 39)
        .addVariable("down", 40)
        .addVariable("insert", 45)
        .addVariable("delete", 46)
        .addVariable("zero", 48)
        .addVariable("one", 49)
        // NOTE: 50 - 99
        .addVariable("two", 50)
        .addVariable("three", 51)
        .addVariable("four", 52)
        .addVariable("five", 53)
        .addVariable("six", 54)
        .addVariable("seven", 55)
        .addVariable("eight", 56)
        .addVariable("nine", 57)
        .addVariable("semicolon", 59)
        .addVariable("colon", 16 | 59)
        .addVariable("equals", 61)
        .addVariable("plus", 16 | 61)
        .addVariable("a", 65)
        .addVariable("b", 66)
        .addVariable("c", 67)
        .addVariable("d", 68)
        .addVariable("e", 69)
        .addVariable("f", 70)
        .addVariable("g", 71)
        .addVariable("h", 72)
        .addVariable("i", 73)
        .addVariable("j", 74)
        .addVariable("k", 75)
        .addVariable("l", 76)
        .addVariable("m", 77)
        .addVariable("n", 78)
        .addVariable("o", 79)
        .addVariable("p", 80)
        .addVariable("q", 81)
        .addVariable("r", 82)
        .addVariable("s", 83)
        .addVariable("t", 84)
        .addVariable("u", 85)
        .addVariable("v", 86)
        .addVariable("w", 87)
        .addVariable("x", 88)
        .addVariable("y", 89)
        .addVariable("z", 90)
        .addVariable("power", 91)
        .addVariable("right_click", 93)
        .addVariable("zero_numlock", 96)
        .addVariable("one_numlock", 97)
        .addVariable("two_numlock", 98)
        .addVariable("three_numlock", 99)
        // NOTE: 100 - 149
        .addVariable("four_numlock", 100)
        .addVariable("five_numlock", 101)
        .addVariable("six_numlock", 102)
        .addVariable("seven_numlock", 103)
        .addVariable("eight_numlock", 104)
        .addVariable("nine_numlock", 105)
        .addVariable("asterix_numlock", 106)
        .addVariable("plus_numlock", 107)
        .addVariable("minus_numlock", 109)
        .addVariable("period_numlock", 110)
        .addVariable("slash_numlock", 111)
        .addVariable("F1", 112)
        .addVariable("F2", 113)
        .addVariable("F3", 114)
        .addVariable("F4", 115)
        .addVariable("F5", 116)
        .addVariable("F6", 117)
        .addVariable("F7", 118)
        .addVariable("F8", 119)
        .addVariable("F9", 120)
        .addVariable("F10", 121)
        .addVariable("F11", 122)
        .addVariable("F12", 123)
        .addVariable("num_lock", 144)
        .addVariable("scroll_lock", 145)
        // NOTE: 150 - 199
        .addVariable("comma", 188)
        .addVariable("less_then", 16 | 188)
        .addVariable("period", 190)
        .addVariable("greater_then", 16 | 190)
        .addVariable("slash", 191)
        .addVariable("question_mark", 16 | 191)
        .addVariable("tick", 192)
        .addVariable("tilde", 16 | 192)
        // NOTE: 200 - 249
        // GET NAMES FOR THESE
        .addVariable("lbracket", 219)
        .addVariable("lbrace", 16 | 219)
        .addVariable("backslash", 220)
        .addVariable("pipe", 16 | 220)
        .addVariable("rbracket", 221)
        .addVariable("rbrace", 16 | 221)
        .addVariable("single_quote", 222)
        .addVariable("double_quote", 16 | 222)

        .endNamespace()
        .endNamespace();
}
