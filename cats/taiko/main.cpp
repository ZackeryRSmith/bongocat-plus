#include "header.hpp"

namespace osuTaiko {
Json::Value rim_key_value[2], center_key_value[2];
sf::Sprite bg, up[2], rim[2], center[2];

int key_state[2] = {0, 0};
bool rim_key_state[2] = {false, false};
bool center_key_state[2] = {false, false};
double timer_rim_key[2] = {-1, -1};
double timer_center_key[2] = {-1, -1};

bool init() {
    // getting configs
    Json::Value taiko = data::cfg["cats"]["taiko"];

    rim_key_value[0] = taiko["leftRim"];
    center_key_value[0] = taiko["leftCenter"];
    rim_key_value[1] = taiko["rightRim"];
    center_key_value[1] = taiko["rightCenter"];

    // check for overlapping keybinds
    if (helpers::keys_overlapping({rim_key_value[0], center_key_value[0], rim_key_value[1], center_key_value[1]})) {
        return false;
    }

    /*
     * importing sprites
     */
    // TODO: Load sprites relitivly to avoid needing to hard code cats/{catname}/img/{file}
    bg.setTexture(data::load_texture("cats/taiko/img/bg.png"));
    up[0].setTexture(data::load_texture("cats/taiko/img/leftup.png"));
    rim[0].setTexture(data::load_texture("cats/taiko/img/leftrim.png"));
    center[0].setTexture(data::load_texture("cats/taiko/img/leftcenter.png"));
    up[1].setTexture(data::load_texture("cats/taiko/img/rightup.png"));
    rim[1].setTexture(data::load_texture("cats/taiko/img/rightrim.png"));
    center[1].setTexture(data::load_texture("cats/taiko/img/rightcenter.png"));

    return true;
}

void draw(const sf::RenderStates& rstates) {
    window.draw(bg, rstates);

    // 0 for left side, 1 for right side
    for (int i = 0; i < 2; i++) {
        // rim
        bool rim_key = helpers::is_pressed(rim_key_value[i]);
        
        if (rim_key) {
            if (!rim_key_state[i]) {
                key_state[i] = 1;
                rim_key_state[i] = true;
            }
        } else {
            rim_key_state[i] = false;
        }

        // center
        bool center_key = helpers::is_pressed(center_key_value[i]);
        
        if (center_key) {
            if (!center_key_state[i]) {
                key_state[i] = 2;
                center_key_state[i] = true;
            }
        } else {
            center_key_state[i] = false;
        }


        /*
         * drawing key states & other sprites
         */
        if (!rim_key_state[i] && !center_key_state[i]) {
            key_state[i] = 0;
            window.draw(up[i], rstates);
        }
        if (key_state[i] == 1) {
            if ((clock() - timer_center_key[i]) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(rim[i], rstates);
                timer_rim_key[i] = clock();
            } else {
                window.draw(up[i], rstates);
            }
        } else if (key_state[i] == 2) {
            if ((clock() - timer_rim_key[i]) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(center[i], rstates);
                timer_center_key[i] = clock();
            } else {
                window.draw(up[i], rstates);
            }
        }
    }
}
}; // namespace taiko
