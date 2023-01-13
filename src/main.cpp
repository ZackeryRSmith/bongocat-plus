#include "header.hpp"

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
    while (!data::init()) {
        continue;
    }

    Json::Value window_width = data::cfg["windowWidth"];
    Json::Value window_height = data::cfg["windowHeight"];

    window.create(sf::VideoMode(window_width.asInt(), window_height.asInt()), "Bongo Cat for osu!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(MAX_FRAMERATE);

    // initialize input
    if (!input::init()) {
        return EXIT_FAILURE;
    }

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

        // selected cat
        int cat = data::cfg["cat"].asInt();

        Json::Value background_color = data::cfg["decoration"]["backgroundColor"];
        int red_value = background_color[0].asInt();
        int green_value = background_color[1].asInt();
        int blue_value = background_color[2].asInt();
        int alpha_value = background_color.size() == 3 ? 255 : background_color[3].asInt();

        // translation for artifacts when resize is applied
        sf::Transform transform = sf::Transform();
        transform.translate(0, window_height.asInt() - BASE_HEIGHT);
        sf::RenderStates rstates = sf::RenderStates(transform);

        window.clear(sf::Color(red_value, green_value, blue_value, alpha_value));
        switch (cat) {
            case 1: osu::draw(rstates);
                    break;
            case 2: osuTaiko::draw(rstates);
                    break;
            case 3: osuCatch::draw(rstates);
                    break;
            case 4: osuMania::draw(rstates);
                    break;
            case 5: custom::draw(rstates);
                    break;
        }

        if (is_show_input_debug) {
            input::drawDebugPanel();
        }

        window.display();
    }

    input::cleanup();
    return 0;
}

