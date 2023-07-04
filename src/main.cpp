#include "header.hpp"
#include <iostream>
#include <memory>

#if _WIN32
    #include <windows.h>
#endif

sf::RenderWindow window;
std::shared_ptr<Cat> cat;

bool is_reload = false;
bool is_show_input_debug = false;

// used if window is borderless
bool borderless;
sf::Vector2i grabbed_offset;
bool grabbed_window = false;

/*****************
 * CREATE WINDOW *
 *****************/
void createWindow() {
    window.create(
        sf::VideoMode(cat->window_width, cat->window_height),
        "BongoCat+",
        borderless ? sf::Style::None : sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(MAX_FRAMERATE);
}

/*****************
 * RELOAD WINDOW *
 *****************/
void reloadWindow() {
    input::cleanup();

    cat = data::init();
    borderless = data::cfg["decoration"]["borderless"].asBool();

    createWindow();

    input::init(cat);
}

/******************
 * PROCESS EVENTS *
 ******************/
void processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // reload window
                if (event.key.code == sf::Keyboard::R && event.key.control) {
                    window.close();
                    is_reload = true;
                    break;
                }

                // toggle debug panel
                if (event.key.code == sf::Keyboard::D && event.key.control) {
                    is_show_input_debug = !is_show_input_debug;
                    break;
                }

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left && borderless) {
                    grabbed_offset = window.getPosition() - sf::Mouse::getPosition();
                    grabbed_window = true;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left && borderless)
                    grabbed_window = false;
                break;

            case sf::Event::MouseMoved:
                if (grabbed_window && borderless)
                    window.setPosition(sf::Mouse::getPosition() + grabbed_offset);
                break;

            default:
                is_reload = false;
        }
    }
}

/**********
 * RENDER *
 **********/
void render() {
    // translation for artifacts when resize is applied
    sf::Transform transform = sf::Transform();
    transform.translate(0, cat->window_height - BASE_HEIGHT);
    sf::RenderStates rstates = sf::RenderStates(transform);
        
    Json::Value background_color = data::cfg["decoration"]["backgroundColor"];
        
    int red_value = background_color[0].asInt();
    int green_value = background_color[1].asInt();
    int blue_value = background_color[2].asInt();
    int alpha_value = background_color.size() == 3 ? 255 : background_color[3].asInt();
        
    window.clear(sf::Color(red_value, green_value, blue_value, alpha_value));
    
    cat->draw(rstates);

    if (is_show_input_debug) {
        input::drawDebugPanel();
    }

    window.display();
}

/*************
 * MAIN LOOP *
 *************/
#if defined(__unix__) || defined(__unix) || __APPLE__
int main(int argc, char ** argv) {
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    FreeConsole(); // maybe not the best option because the console is still 
                   // running with the console subsystem, but it does hide the console
#endif
    cat = data::init();
    borderless = data::cfg["decoration"]["borderless"].asBool();

    createWindow();
    input::init(cat);

    while (window.isOpen()) {
        processEvents();

        if (is_reload) {
            reloadWindow();
            is_reload = false;  // reset the reload flag
        }

        render();
    }

    input::cleanup();
    return 0;
}
