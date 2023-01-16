#pragma once
#define BONGO_KEYPRESS_THRESHOLD 0
#define BASE_WIDTH 612
#define BASE_HEIGHT 352
#define MAX_FRAMERATE 60

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include <time.h>

#include <math.h>
#include <string.h>

#include <SFML/Graphics.hpp>
#include "json/json.h"

extern sf::RenderWindow window;

namespace data {
extern Json::Value cfg;

void error_msg(std::string error, std::string title);

bool init();

sf::Texture &load_texture(std::string path);
}; // namespace data

namespace input {
bool init();

bool is_pressed(int key_code);

bool is_joystick_connected();
bool is_joystick_pressed(int key_code);

std::pair<double, double> bezier(double ratio, std::vector<double> &points, int length);

std::pair<double, double> get_xy();

void drawDebugPanel();

void cleanup();
}; // namespace input

namespace helpers {
bool is_pressed(Json::Value key_value);
bool keys_overlapping(std::vector<Json::Value> key_arrays);
}

namespace osu {
bool init();

void draw(const sf::RenderStates &rstates);
}; // namespace osu

namespace osuTaiko {
bool init();

void draw(const sf::RenderStates &rstates);
}; // namespace taiko

namespace osuCatch {
bool init();

void draw(const sf::RenderStates &rstates);
}; // namespace ctb

namespace osuMania {
bool init();

void draw(const sf::RenderStates &rstates);
}; // namespace mania
