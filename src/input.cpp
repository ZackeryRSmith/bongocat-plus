#include "RobotoMonoBold.hpp"
#include "header.hpp"
#include <SFML/Window.hpp>
#include <cmath>
#include <iomanip>
#include <sstream>

#if _WIN32
#include <windows.h>
#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#elif defined(__unix__) || defined(__unix)
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/keysym.h>

extern "C" {
#include <xdo.h>
}
#endif

#define TOTAl_INPUT_TABLE_SIZE 256

namespace input {
int horizontal, vertical;
int osu_x, osu_y, osu_h, osu_v;
bool is_letterbox, is_left_handed;

std::string debugMessage;
std::string debugBitMessage;

sf::RectangleShape debugBackground;
sf::Font debugFont;
sf::Text debugText;

#if defined(__unix__) || defined(__unix)
xdo_t *xdo;
Display *dpy;
Window foreground_window;

static int _XlibErrorHandler(Display *display, XErrorEvent *event) {
    return true;
}
#endif

int INPUT_KEY_TABLE[TOTAl_INPUT_TABLE_SIZE];

// void init(std::shared_ptr<Cat> cat) {
void init() {
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

    INPUT_KEY_TABLE[27] = (int)sf::Keyboard::Key::Escape;
    INPUT_KEY_TABLE[17] = (int)sf::Keyboard::Key::LControl;
    INPUT_KEY_TABLE[16] = (int)sf::Keyboard::Key::LShift;
    INPUT_KEY_TABLE[18] = (int)sf::Keyboard::Key::LAlt;
    INPUT_KEY_TABLE[17] = (int)sf::Keyboard::Key::RControl;
    INPUT_KEY_TABLE[16] = (int)sf::Keyboard::Key::RShift;
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

    is_letterbox = data::cfg["resolution"]["letterboxing"].asBool();
    osu_x = data::cfg["resolution"]["width"].asInt();
    osu_y = data::cfg["resolution"]["height"].asInt();
    osu_h = data::cfg["resolution"]["horizontalPosition"].asInt();
    osu_v = data::cfg["resolution"]["verticalPosition"].asInt();
    is_left_handed = data::cfg["decoration"]["leftHanded"].asBool();

#if _WIN32
    // getting resolution
    RECT desktop;
    const HWND h_desktop = GetDesktopWindow();
    GetWindowRect(h_desktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
#elif __APPLE__
    // Get the main display
    CGDirectDisplayID displayID = kCGDirectMainDisplay;
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayID);

    int width = CGDisplayModeGetWidth(mode);
    int height = CGDisplayModeGetHeight(mode);

    CGDisplayModeRelease(mode);

    horizontal = width;
    vertical = height;
#elif defined(__unix__) || defined(__unix)
    // Set x11 error handler
    XSetErrorHandler(_XlibErrorHandler);

    // Get desktop resolution
    int num_sizes;
    Rotation current_rotation;

    dpy = XOpenDisplay(NULL);
    Window root = RootWindow(dpy, 0);
    XRRScreenSize *xrrs = XRRSizes(dpy, 0, &num_sizes);

    XRRScreenConfiguration *conf = XRRGetScreenInfo(dpy, root);
    SizeID current_size_id =
        XRRConfigCurrentConfiguration(conf, &current_rotation);

    int width = xrrs[current_size_id].width;
    int height = xrrs[current_size_id].height;

    horizontal = width;
    vertical = height;

    xdo = xdo_new(NULL);
#endif

    // loading font
    debugFont.loadFromMemory(&RobotoMono_Bold_ttf, RobotoMono_Bold_ttf_len);

    // initialize debug resource
    // debugBackground.setSize(sf::Vector2f(cat->window_width,
    // cat->window_height)); debugBackground.setFillColor(sf::Color(0, 0, 0,
    // 128));

    debugText.setFont(debugFont);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(10.0f, 4.0f);
    debugText.setString(debugMessage);
}

sf::Keyboard::Key ascii_to_key(int key_code) {
    if (key_code < 0 || key_code >= TOTAl_INPUT_TABLE_SIZE) {
        // out of range
        return sf::Keyboard::Unknown;
    } else {
        return (sf::Keyboard::Key)(INPUT_KEY_TABLE[key_code]);
    }
}

// for some special cases of num dot and such
bool is_pressed_fallback(int key_code) {
#if _WIN32
    return (GetAsyncKeyState(key_code) & 0x8000) != 0;
#elif __APPLE__
    UInt32 key_map[4] = {0};
    CGEventRef event =
        CGEventCreateKeyboardEvent(NULL, (CGKeyCode)key_code, true);
    CGEventFlags flags = CGEventGetFlags(event);
    CFRelease(event);
    return (flags & kCGEventFlagMaskCommand) != 0;
#elif defined(__unix__) || defined(__unix) // code snippet from SFML
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

bool is_pressed(int key_code) {
    if (key_code == 16) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    } else if (key_code == 17) {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    } else {
        sf::Keyboard::Key selected = ascii_to_key(key_code);
        if (selected != sf::Keyboard::Key::Unknown) {
            return sf::Keyboard::isKeyPressed(selected);
        } else {
            return is_pressed_fallback(key_code);
        }
    }
}

std::pair<double, double> get_xy() {
#if _WIN32
    double letter_x = 0;
    double letter_y = 0;
    double s_height = horizontal;
    double s_width = vertical;

    HWND handle = GetForegroundWindow();
    if (handle) {
        RECT windowRect;
        GetWindowRect(handle, &windowRect);
        s_width = windowRect.right - windowRect.left;
        s_height = windowRect.bottom - windowRect.top;
        letter_x = windowRect.left;
        letter_y = windowRect.top;
    } else {
        s_width = horizontal;
        s_height = vertical;
        letter_x = 0;
        letter_y = 0;
    }

    double x, y;
    POINT point;
    if (GetCursorPos(&point)) {
        double fx = (1.0 * point.x - letter_x) / s_width;
        if (is_left_handed) {
            fx = 1 - fx;
        }
        double fy = (1.0 * point.y - letter_y) / s_height;
        fx = std::min(fx, 1.0);
        fx = std::max(fx, 0.0);
        fy = std::min(fy, 1.0);
        fy = std::max(fy, 0.0);
        x = -97 * fx + 44 * fy + 184;
        y = -76 * fx - 40 * fy + 324;
    } else {
        x = -1;
        y = -1;
    }
#elif __APPLE__
    double x, y;

    // getting device resolution
    double s_height, s_width;
    CGRect displayBounds = CGDisplayBounds(CGMainDisplayID());
    s_height = displayBounds.size.height;
    s_width = displayBounds.size.width;

    // getting mouse position
    CGPoint mousePos;
    CGEventRef event = CGEventCreate(NULL);
    mousePos = CGEventGetLocation(event);
    CFRelease(event);

    // getting window under mouse cursor
    CFArrayRef windows = CGWindowListCopyWindowInfo(
        kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    CFIndex count = CFArrayGetCount(windows);
    for (CFIndex i = 0; i < count; i++) {
        CFDictionaryRef window =
            (CFDictionaryRef)CFArrayGetValueAtIndex(windows, i);
        CFStringRef name =
            (CFStringRef)CFDictionaryGetValue(window, kCGWindowName);
        if (name && CFGetTypeID(name) == CFStringGetTypeID() &&
            CFStringGetLength(name) > 0) {
            CGRect bounds;
            CFDictionaryRef boundsDict =
                (CFDictionaryRef)CFDictionaryGetValue(window, kCGWindowBounds);
            CGRectMakeWithDictionaryRepresentation(boundsDict, &bounds);
            if (CGRectContainsPoint(bounds, mousePos)) {
                double fx = (mousePos.x - bounds.origin.x) / bounds.size.width;
                double fy = (mousePos.y - bounds.origin.y) / bounds.size.height;
                fx = std::min(fx, 1.0);
                fx = std::max(fx, 0.0);
                fy = std::min(fy, 1.0);
                fy = std::max(fy, 0.0);
                x = -97 * fx + 44 * fy + 184;
                y = -76 * fx - 40 * fy + 324;
            }
        }
    }
    CFRelease(windows);
#elif defined(__unix__) || defined(__unix)
    int x_pos = 0;
    int y_pos = 0;
    double s_height = horizontal;
    double s_width = vertical;

    double x = 0, y = 0;
    int px = 0, py = 0;

    if (xdo_get_mouse_location(xdo, &px, &py, NULL) == 0) {
        Window window_under_cursor;
        if (xdo_get_active_window(xdo, &window_under_cursor) == 0) {
            unsigned int width_ret, height_ret;
            if (xdo_get_window_size(xdo, window_under_cursor, &width_ret,
                                    &height_ret) == 0) {
                s_width = width_ret;
                s_height = height_ret;
            }
        }

        xdo_get_window_location(xdo, window_under_cursor, &x_pos, &y_pos, NULL);

        /**
         * Ratio of the mouse position to the window size on the x axis
         */
        double fx = (1.0 * px - x_pos) / s_width;

        if (is_left_handed) {
            fx = 1 - fx;
        }

        /**
         * Ratio of the mouse position to the window size on the y axis
         */
        double fy = (1.0 * py - y_pos) / s_height;

        fx = std::min(fx, 1.0);
        fx = std::max(fx, 0.0);

        fy = std::min(fy, 1.0);
        fy = std::max(fy, 0.0);

        /**
         * Magic numbers to position the mouse cursor on the mouse pad correctly
         */
        x = -97 * fx + 44 * fy + 184;
        y = -76 * fx - 40 * fy + 324;
    }
#endif

    return std::make_pair(x, y);
}

void drawDebugPanel() {
    // std::stringstream result;

    // result << "Joystick connected : " << std::endl;
    // result << "Support button : " << std::endl;

    sf::Text text("WORKS!!!!!!YIPPPEEEEEE\n\nOh yeah, see #12", debugFont);

    window.draw(debugBackground);
    window.draw(text);
}

void cleanup() {
#if defined(__unix__) || defined(__unix)
    delete xdo;
    XCloseDisplay(dpy);
#endif
}

}; // namespace input
