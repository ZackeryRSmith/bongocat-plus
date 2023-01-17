#include "header.hpp"

class OsuMania : public Cat {
    public:
        sf::Sprite bg, left_handup, right_handup, left_hand[3], right_hand[3];
        sf::Sprite left_4K[2], right_4K[2], left_7K[4], right_7K[4];
        int left_key_value_4K[2], right_key_value_4K[2];
        int left_key_value_7K[4], right_key_value_7K[4];
        bool is_4K;

    OsuMania() {
        // getting configs
        Json::Value cfg = data::cfg["cats"]["mania"];

        std::cout << helpers::get_window_size(cfg)[0] << std::endl;
        std::cout << helpers::get_window_size(cfg)[1] << std::endl;
        fflush(stdout);

        window_width = helpers::get_window_size(cfg)[0];
        window_height = helpers::get_window_size(cfg)[1];

        is_4K = cfg["4K"].asBool();

        // TODO: Clean this code
        for (int i = 0; i < 2; i++) {
            left_key_value_4K[i] = cfg["key4k"].isInt() ? cfg["key4k"].asInt() : (int)cfg["key4K"][i].asString().at(0);
        }
        for (int i = 0; i < 2; i++) {
            right_key_value_4K[i] = cfg["key4K"][i + 2].isInt() ? cfg["key4K"][i + 2].asInt() : (int)cfg["key4K"][i + 2].asString().at(0);
        }

        for (int i = 0; i < 4; i++) {
            left_key_value_7K[i] = cfg["key7K"][i].isInt() ? cfg["key7K"][i].asInt() : (int)cfg["key7K"][i].asString().at(0);
        }
        for (int i = 0; i < 4; i++) {
            right_key_value_7K[i] = cfg["key7K"][i + 3].isInt() ? cfg["key7K"][i + 3].asInt() : (int)cfg["key7K"][i + 3].asString().at(0);
        }

        /*
         * importing sprites
         */
        left_handup.setTexture(data::load_texture("cats/mania/leftup.png"));
        right_handup.setTexture(data::load_texture("cats/mania/rightup.png"));
        for (int i = 0; i < 3; i++) {
            left_hand[i].setTexture(data::load_texture("cats/mania/left" + std::to_string(i) + ".png"));
            right_hand[i].setTexture(data::load_texture("cats/mania/right" + std::to_string(i) + ".png"));
        }

        if (is_4K) {
            bg.setTexture(data::load_texture("cats/mania/4K/bg.png"));
            for (int i = 0; i < 2; i++) {
                left_4K[i].setTexture(data::load_texture("cats/mania/4K/" + std::to_string(i) + ".png"));
            }
            for (int i = 0; i < 2; i++) {
                right_4K[i].setTexture(data::load_texture("cats/mania/4K/" + std::to_string(i + 2) + ".png"));
            }
        } else {
            bg.setTexture(data::load_texture("cats/mania/7K/bg.png"));
            for (int i = 0; i < 4; i++) {
                left_7K[i].setTexture(data::load_texture("cats/mania/7K/" + std::to_string(i) + ".png"));
            }
            for (int i = 0; i < 4; i++) {
                right_7K[i].setTexture(data::load_texture("cats/mania/7K/" + std::to_string(i + 3) + ".png"));
            }
        }
    }

    void draw_4K(const sf::RenderStates& rstates) {
        bg.setPosition(0, window_height - bg.getTextureRect().height);
        window.draw(bg);

        int left_cnt = 0, right_cnt = 0;
        int left_sum = 0, right_sum = 0;

        for (int i = 0; i < 2; i++) {
            if (input::is_pressed(left_key_value_4K[i])) {
                window.draw(left_4K[i], rstates);
                left_cnt++;
                left_sum += i;
            }
            if (input::is_pressed(right_key_value_4K[i])) {
                window.draw(right_4K[i], rstates);
                right_cnt++;
                right_sum += i;
            }
        }

        // draw left hand
        if (left_cnt == 0) {
            window.draw(left_handup, rstates);
        } else {
            double avg = 1.0 * left_sum / left_cnt;
            if (avg == 0) {
                window.draw(left_hand[0], rstates);
            } else if (avg == 0.5) {
                window.draw(left_hand[1], rstates);
            } else {
                window.draw(left_hand[2], rstates);
            }
        }

        // draw right hand
        if (right_cnt == 0) {
            window.draw(right_handup, rstates);
        } else {
            double avg = 1.0 * right_sum / right_cnt;
            if (avg == 0) {
                window.draw(right_hand[0], rstates);
            } else if (avg == 0.5) {
                window.draw(right_hand[1], rstates);
            } else {
                window.draw(right_hand[2], rstates);
            }
        }
    }

    void draw_7K(const sf::RenderStates& rstates) {
        bg.setPosition(0, window_height - bg.getTextureRect().height);
        window.draw(bg);

        int left_cnt = 0, right_cnt = 0;
        int left_sum = 0, right_sum = 0;

        for (int i = 0; i < 4; i++) {
            if (input::is_pressed(left_key_value_7K[i])) {
                window.draw(left_7K[i], rstates);
                left_cnt++;
                left_sum += i;
            }
            if (input::is_pressed(right_key_value_7K[i])) {
                window.draw(right_7K[i], rstates);
                right_cnt++;
                right_sum += i;
            }
        }

        // draw left hand
        if (left_cnt == 0) {
            window.draw(left_handup, rstates);
        } else {
            double avg = 1.0 * left_sum / left_cnt;
            if (avg < 1.0) {
                window.draw(left_hand[0], rstates);
            } else if (avg <= 2.0) {
                window.draw(left_hand[1], rstates);
            } else {
                window.draw(left_hand[2], rstates);
            }
        }

        // draw right hand
        if (right_cnt == 0) {
            window.draw(right_handup, rstates);
        } else {
            double avg = 1.0 * right_sum / right_cnt;
            if (avg < 1.0) {
                window.draw(right_hand[0], rstates);
            } else if (avg <= 2.0) {
                window.draw(right_hand[1], rstates);
            } else {
                window.draw(right_hand[2], rstates);
            }
        }
    }

    void draw(const sf::RenderStates& rstates) override {
        if (is_4K) {
            draw_4K(rstates);
        } else {
            draw_7K(rstates);
        }
    }
};
