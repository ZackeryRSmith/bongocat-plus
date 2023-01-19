#include "header.hpp"
#include <iostream>
#include <memory>

#if !defined(__unix__) && !defined(__unix)
    #include <windows.h>
#endif


sf::RenderWindow window;

#if defined(__unix__) || defined(__unix)
int main(int argc, char ** argv) {
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif

    // loading configs
    std::shared_ptr<Cat> cat = data::init();
    
    window.create(sf::VideoMode(cat->window_width, cat->window_height), "Bongo Cat for osu!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(MAX_FRAMERATE);

    // initialize input
    input::init(cat);

    bool is_reload = false;
    bool is_show_input_debug = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // get reload config prompt
                if (event.key.code == sf::Keyboard::R && event.key.control) {
                    if (!is_reload) {
                        while (!data::init()) {
                            continue;
                        }
                    }
                    is_reload = true;
                    break;
                }

                // toggle joystick debug panel
                if (event.key.code == sf::Keyboard::D && event.key.control) {
                    is_show_input_debug = !is_show_input_debug;
                    break;
                }

            default:
                is_reload = false;
            }
        }

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

    input::cleanup();
    return 0;
}

