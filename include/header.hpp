#ifndef HEADER_CPP
#define HEADER_CPP

#define BONGO_KEYPRESS_THRESHOLD 0
#define BASE_WIDTH 612
#define BASE_HEIGHT 354
#define MAX_FRAMERATE 60

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include <stdexcept>
#include <memory>

#include <time.h>

#include <math.h>
#include <string.h>

#include <SFML/Graphics.hpp>
#include "json/json.h"

extern sf::RenderWindow window;

namespace data {
extern Json::Value cfg;

void error_msg(std::string error, std::string title);

sf::Texture &load_texture(std::string path);
}; // namespace data

namespace input {
//void init(std::shared_ptr<Cat> cat);
void init();

bool is_pressed(int key_code);

std::pair<double, double> get_xy();

void drawDebugPanel();

void cleanup();
}; // namespace input

namespace helpers {
bool is_pressed(Json::Value key_value);
bool keys_overlapping(std::vector<Json::Value> key_arrays);

std::vector<int> get_window_size(Json::Value cat_config);

std::pair<double, double> bezier(double ratio, std::vector<double> &points, int length);
}
#endif