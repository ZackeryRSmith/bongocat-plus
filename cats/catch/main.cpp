#include "header.hpp"

namespace osuCatch {
Json::Value left_key_value, right_key_value, dash_key_value;
sf::Sprite bg, mid, left, right, dash, up;

int key_state = 0;
bool left_key_state = false;
bool right_key_state = false;
double timer_left_key = -1;
double timer_right_key = -1;

bool init() {
    // getting configs
    Json::Value osuCatch = data::cfg["cats"]["catch"];

    bool chk[256];
    std::fill(chk, chk + 256, false);
    left_key_value = osuCatch["left"];
    for (Json::Value &v : left_key_value) {
        chk[v.asInt()] = true;
    }
    right_key_value = osuCatch["right"];
    for (Json::Value &v : right_key_value) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osu!catch keybinds", "Error reading configs");
            return false;
        }
    }
    dash_key_value = osuCatch["dash"];

    /*
     * importing sprites
     */
    // TODO: Load sprites relitivly to avoid needing to hard code cats/{catname}/img/{file}
    bg.setTexture(data::load_texture("cats/catch/img/bg.png"));
    mid.setTexture(data::load_texture("cats/catch/img/mid.png"));
    left.setTexture(data::load_texture("cats/catch/img/left.png"));
    right.setTexture(data::load_texture("cats/catch/img/right.png"));
    dash.setTexture(data::load_texture("cats/catch/img/dash.png"));
    up.setTexture(data::load_texture("cats/catch/img/up.png"));

    return true;
}

void draw(const sf::RenderStates& rstates) {
    window.draw(bg);
    
    // drawing left-right keypresses
    bool left_key = false;
    for (Json::Value &v : left_key_value) {
        if (input::is_pressed(v.asInt())) {
            left_key = true;
            break;
        }
    }
    if (left_key) {
        if (!left_key_state) {
            key_state = 1;
            left_key_state = true;
        }
    } else {
        left_key_state = false;
    }

    bool right_key = false;
    for (Json::Value &v : right_key_value) {
        if (input::is_pressed(v.asInt())) {
            right_key = true;
            break;
        }
    }
    if (right_key) {
        if (!right_key_state) {
            key_state = 2;
            right_key_state = true;
        }
    } else {
        right_key_state = false;
    }

    if (!left_key_state && !right_key_state) {
        key_state = 0;
        window.draw(mid, rstates);
    }
    if (key_state == 1) {
        if ((clock() - timer_right_key) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            window.draw(left, rstates);
            timer_left_key = clock();
        } else {
            window.draw(mid, rstates);
        }
    } else if (key_state == 2) {
        if ((clock() - timer_left_key) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            window.draw(right, rstates);
            timer_right_key = clock();
        } else {
            window.draw(mid, rstates);
        }
    }

    bool is_dash = false;
    for (Json::Value &v : dash_key_value) {
        if (input::is_pressed(v.asInt())) {
            window.draw(dash, rstates);
            is_dash = true;
            break;
        }
    }
    if (!is_dash) {
        window.draw(up, rstates);
    }
}
}; // namespace osuCatch
