/**************************************************************

        A hand-full of functions to help facilitate the
             the development of the cats c++ file!

**************************************************************/

#include "header.hpp"
#include <vector>
namespace helpers {

// Wrapper function for input::is_pressed
//   * Automaticly converts ASCII chars to ASCII codes
bool is_pressed(Json::Value key_value) {
    for (Json::Value &v : key_value) {
        if (input::is_pressed(v.isInt() ? v.asInt() : (int)v.asString().at(0))) {
            return true;
        }
    }
    return false;
}

// Code cleaner. Makes checking for overlapping keys a breeze
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
};
