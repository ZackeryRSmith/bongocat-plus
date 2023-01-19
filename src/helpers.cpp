/**************************************************************

        A hand-full of functions to help facilitate the
             the development of the cats c++ file!

**************************************************************/

#include "header.hpp"
#include <vector>
#include <tuple>
namespace helpers {

// wrapper function for input::is_pressed
//   * Automaticly converts ASCII chars to ASCII codes
bool is_pressed(Json::Value key_value) {
    for (Json::Value &v : key_value) {
        if (input::is_pressed(v.isInt() ? v.asInt() : (int)v.asString().at(0))) {
            return true;
        }
    }
    return false;
}

// code cleaner. Makes checking for overlapping keys a breeze
//   * Automaticly converts ASCII chars to ASCII codes
bool keys_overlapping(std::vector<Json::Value> key_arrays) {
    bool chk[256]{}; // check buffer, used to keep track of keys
    int v;

    for (Json::Value &key_array : key_arrays) {
        for (Json::Value &tmp : key_array) {
            v = tmp.isInt() ? tmp.asInt() : (int)tmp.asString().at(0);

            if (chk[v]) { return true; }
            chk[v] = true;
        }
    }

    return false;
}

// returns default size values defined in header.hpp if
// "windowWidth" or "windowHeight" aren't defined in the cats config
std::vector<int> get_window_size(Json::Value cat_config) {
    int window_width = cat_config["windowWidth"].isNull() ? BASE_WIDTH : cat_config["windowWidth"].asInt();
    int window_height = cat_config["windowHeight"].isNull() ? BASE_HEIGHT : cat_config["windowHeight"].asInt();

    return {window_width, window_height};
}

// bezier curve
std::pair<double, double> bezier(double ratio, std::vector<double> &points, int length) {
    double fact[22] = {0.001, 0.001, 0.002, 0.006, 0.024, 0.12, 0.72, 5.04, 40.32, 362.88, 3628.8, 39916.8, 479001.6, 6227020.8, 87178291.2, 1307674368.0, 20922789888.0, 355687428096.0, 6402373705728.0, 121645100408832.0, 2432902008176640.0, 51090942171709440.0};
    int nn = (length / 2) - 1;
    double xx = 0;
    double yy = 0;

    for (int point = 0; point <= nn; point++) {
        double tmp = fact[nn] / (fact[point] * fact[nn - point]) * pow(ratio, point) * pow(1 - ratio, nn - point);
        xx += points[2 * point] * tmp;
        yy += points[2 * point + 1] * tmp;
    }

    return std::make_pair(xx / 1000, yy / 1000);
}
};
