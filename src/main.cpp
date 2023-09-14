#include "BocAPI.hpp"
#include "header.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

#if _WIN32
#include <windows.h>
#endif

sf::RenderWindow window;
// std::shared_ptr<Cat> cat;

// used for keeping window consistant
// even through reloads
sf::Vector2i windowPosition;

bool is_show_input_debug = false;

// used if window is borderless
bool borderless;
sf::Vector2i grabbed_offset;
bool grabbed_window = false;

bool auto_reload;
bool should_exit_monitor = false;

/*****************
 * CREATE WINDOW *
 *****************/
void createWindow() {
    window.create(
        // sf::VideoMode(cat->window_width, cat->window_height),
        sf::VideoMode(BASE_WIDTH, BASE_HEIGHT), "BongoCat+",
        borderless ? sf::Style::None : sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(MAX_FRAMERATE);
    window.setPosition(windowPosition);
}

/*****************
 * RELOAD WINDOW *
 *****************/
void reloadWindow() {
    input::cleanup();

    // cat = data::init();
    borderless = data::cfg["decoration"]["borderless"].asBool();
    auto_reload = data::cfg["auto-reload"].asBool();

    windowPosition = window.getPosition();
    createWindow();

    // input::init(cat);
    input::init();
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
                reloadWindow();
                break;
            }

            // toggle debug panel
            if (event.key.code == sf::Keyboard::D && event.key.control) {
                is_show_input_debug = !is_show_input_debug;
                break;
            }

        // for mice this system works great, for trackpads however not so
        // much... As trackpads have acceleration the faster you move you can
        // drag your mouse off the window.
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left && borderless) {
                grabbed_offset =
                    window.getPosition() - sf::Mouse::getPosition();
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
            break;
        }
    }
}

/**********
 * RENDER *
 **********/
void render() {
    // translation for artifacts when resize is applied
    sf::Transform transform = sf::Transform();
    // transform.translate(0, cat->window_height - BASE_HEIGHT);
    transform.translate(0, BASE_HEIGHT);
    sf::RenderStates rstates = sf::RenderStates(transform);

    Json::Value background_color = data::cfg["decoration"]["backgroundColor"];

    int red_value = background_color[0].asInt();
    int green_value = background_color[1].asInt();
    int blue_value = background_color[2].asInt();
    int alpha_value =
        background_color.size() == 3 ? 255 : background_color[3].asInt();

    window.clear(sf::Color(red_value, green_value, blue_value, alpha_value));

    // cat->draw(rstates);

    if (is_show_input_debug) {
        input::drawDebugPanel();
    }

    window.display();
}

/***********************
 * MONITOR CONFIG FILE *
 ***********************/
void monitorConfigFile() {
    std::filesystem::path configPath = "config.json";
    auto lastModifiedTime = std::filesystem::last_write_time(configPath);

    while (!should_exit_monitor) {
        // sleep for a short duration
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // check if the config file has been modified
        auto currentModifiedTime = std::filesystem::last_write_time(configPath);

        if (currentModifiedTime != lastModifiedTime) {
            lastModifiedTime = currentModifiedTime;
            if (auto_reload)
                reloadWindow();
        }
    }
}

/*************
 * MAIN LOOP *
 *************/
#if defined(__unix__) || defined(__unix) || __APPLE__
int main(int argc, char **argv) {
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    FreeConsole(); // maybe not the best option because the console is still
                   // running with the console subsystem, but it does hide the
                   // console
#endif
    std::thread monitorConfigFileThread(monitorConfigFile);

    // cat = data::init();
    borderless = data::cfg["decoration"]["borderless"].asBool();
    auto_reload = data::cfg["auto-reload"].asBool();

    createWindow();
    // input::init(cat);
    input::init();

    while (window.isOpen()) {
        processEvents();
        render();
    }

    should_exit_monitor = true;
    monitorConfigFileThread.join();
    input::cleanup();
    return 0;
}
