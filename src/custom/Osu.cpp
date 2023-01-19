#include "header.hpp"

class Osu : public Cat {
    public:
        Json::Value left_key_value, right_key_value, smoke_key_value, wave_key_value;
        int offset_x, offset_y;
        int x_paw_start, y_paw_start, x_paw_end, y_paw_end;
        int paw_arc1_width, paw_arc2_width;
        int paw_r, paw_g, paw_b, paw_a;
        int paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a;
        double scale;
        bool is_mouse, is_left_handed, is_enable_toggle_smoke;
        sf::Sprite bg, up, left, right, device, smoke, wave;

        int key_state = 0;

        bool left_key_state = false;
        bool right_key_state = false;
        bool wave_key_state = false;
        bool previous_smoke_key_state = false;
        bool current_smoke_key_state = false;
        bool is_toggle_smoke = false;
        double timer_left_key = -1;
        double timer_right_key = -1;
        double timer_wave_key = -1;

    Osu() {
        /*
         * load config
         */
        Json::Value cfg = data::cfg["cats"]["osu"];

        window_width = helpers::get_window_size(cfg)[0];
        window_height = helpers::get_window_size(cfg)[1];
        
        is_mouse = cfg["mouse"].asBool();
        is_enable_toggle_smoke = cfg["toggleSmoke"].asBool();

        x_paw_start = cfg["pawStartingPoint"][0].asInt();
        y_paw_start = cfg["pawStartingPoint"][1].asInt();
        x_paw_end = cfg["pawEndingPoint"][0].asInt();
        y_paw_end = cfg["pawEndingPoint"][1].asInt();

        paw_r = cfg["pawColor"][0].asInt();
        paw_g = cfg["pawColor"][1].asInt();
        paw_b = cfg["pawColor"][2].asInt();
        paw_a = cfg["pawColor"].size() == 3 ? 255 : cfg["pawColor"][3].asInt();

        paw_arc1_width = cfg["pawArc1Width"].asInt(); 
        paw_arc2_width = cfg["pawArc2Width"].asInt();

        paw_edge_r = cfg["pawEdgeColor"][0].asInt();
        paw_edge_g = cfg["pawEdgeColor"][1].asInt();
        paw_edge_b = cfg["pawEdgeColor"][2].asInt();
        paw_edge_a = cfg["pawEdgeColor"].size() == 3 ? 255 : cfg["pawEdgeColor"][3].asInt();

        left_key_value = cfg["key1"];
        right_key_value = cfg["key2"];
        wave_key_value = cfg["wave"];
        smoke_key_value = cfg["smoke"];
    
        // check for overlapping keybinds
        if (helpers::keys_overlapping({left_key_value, right_key_value, wave_key_value, smoke_key_value})) {
            data::error_msg("Overlapping osu! keybinds", "Error reading configs");
            exit(1);
        }

        is_left_handed = data::cfg["decoration"]["leftHanded"].asBool();

        if (is_mouse) {
            offset_x = (data::cfg["decoration"]["offsetX"])[0].asInt();
            offset_y = (data::cfg["decoration"]["offsetY"])[0].asInt();
            scale = (data::cfg["decoration"]["scalar"])[0].asDouble();
        } else {
            offset_x = (data::cfg["decoration"]["offsetX"])[1].asInt();
            offset_y = (data::cfg["decoration"]["offsetY"])[1].asInt();
            scale = (data::cfg["decoration"]["scalar"])[1].asDouble();
        }

        /* 
         * importing sprites
         */
        up.setTexture(data::load_texture("cats/osu/up.png"));
        left.setTexture(data::load_texture("cats/osu/left.png"));
        right.setTexture(data::load_texture("cats/osu/right.png"));
        wave.setTexture(data::load_texture("cats/osu/wave.png"));
        if (is_mouse) {
            bg.setTexture(data::load_texture("cats/osu/mousebg.png"));
            device.setTexture(data::load_texture("cats/osu/mouse.png"), true);
        } else {
            bg.setTexture(data::load_texture("cats/osu/tabletbg.png"));
            device.setTexture(data::load_texture("cats/osu/tablet.png"), true);
        }
        smoke.setTexture(data::load_texture("cats/osu/smoke.png"));
        device.setScale(scale, scale);
    }

    void draw(const sf::RenderStates& rstates) override {
        bg.setPosition(0, window_height - bg.getTextureRect().height);
        window.draw(bg);

        /* 
         * initializing pss and pss2 (kuvster's magic)
         */
        auto [x, y] = input::get_xy();
        int oof = 6;
        std::vector<double> pss = {(float) x_paw_start, (float) y_paw_start};
        double dist = hypot(x_paw_start - x, y_paw_start - y);
        double centerleft0 = x_paw_start - 0.7237 * dist / 2;
        double centerleft1 = y_paw_start + 0.69 * dist / 2;
        for (int i = 1; i < oof; i++) {
            std::vector<double> bez = {(float) x_paw_start, (float) y_paw_start, centerleft0, centerleft1, x, y};
            auto [p0, p1] = helpers::bezier(1.0 * i / oof, bez, 6);
            pss.push_back(p0);
            pss.push_back(p1);
        }
        pss.push_back(x);
        pss.push_back(y);
        double a = y - centerleft1;
        double b = centerleft0 - x;
        double le = hypot(a, b);
        a = x + a / le * 60;
        b = y + b / le * 60;
        dist = hypot(x_paw_end - a, y_paw_end - b);
        double centerright0 = x_paw_end - 0.6 * dist / 2;
        double centerright1 = y_paw_end + 0.8 * dist / 2;
        int push = 20;
        double s = x - centerleft0;
        double t = y - centerleft1;
        le = hypot(s, t);
        s *= push / le;
        t *= push / le;
        double s2 = a - centerright0;
        double t2 = b - centerright1;
        le = hypot(s2, t2);
        s2 *= push / le;
        t2 *= push / le;
        for (int i = 1; i < oof; i++) {
            std::vector<double> bez = {x, y, x + s, y + t, a + s2, b + t2, a, b};
            auto [p0, p1] = helpers::bezier(1.0 * i / oof, bez, 8);
            pss.push_back(p0);
            pss.push_back(p1);
        }
        pss.push_back(a);
        pss.push_back(b);
        for (int i = oof - 1; i > 0; i--) {
            std::vector<double> bez = {1.0 * x_paw_end, 1.0 * y_paw_end, centerright0, centerright1, a, b};
            auto [p0, p1] = helpers::bezier(1.0 * i / oof, bez, 6);
            pss.push_back(p0);
            pss.push_back(p1);
        }
        pss.push_back(x_paw_end);
        pss.push_back(y_paw_end);
        double mpos0 = (a + x) / 2 - 52 - 15;
        double mpos1 = (b + y) / 2 - 34 + 5;
        double dx = -38;
        double dy = -50;

        const int iter = 25;

        std::vector<double> pss2 = {pss[0] + dx, pss[1] + dy};
        for (int i = 1; i < iter; i++) {
            auto [p0, p1] = helpers::bezier(1.0 * i / iter, pss, 38);
            pss2.push_back(p0 + dx);
            pss2.push_back(p1 + dy);
        }
        pss2.push_back(pss[36] + dx);
        pss2.push_back(pss[37] + dy);

        device.setPosition(mpos0 + dx + offset_x, mpos1 + dy + offset_y);

        /*
         * ANYTHING THAT NEEDS TO BE DONE BEFORE ARMS ARE DRAWN GOES HERE
         */
        if (is_mouse) {
            window.draw(device, rstates);
        }

        // smoke
        bool is_smoke_key_pressed = helpers::is_pressed(smoke_key_value);

        if (is_enable_toggle_smoke) {
            previous_smoke_key_state = current_smoke_key_state;
            current_smoke_key_state = is_smoke_key_pressed;

            bool is_smoke_key_down = current_smoke_key_state && (current_smoke_key_state != previous_smoke_key_state);

            if (is_smoke_key_down) {
                is_toggle_smoke = !is_toggle_smoke;
            }
        }
        else {
            is_toggle_smoke = is_smoke_key_pressed;
        }

        if (is_toggle_smoke) {
            window.draw(smoke, rstates);
        }


        /*
         * drawing arms
         */
        sf::VertexArray fill(sf::TriangleStrip, 26);
        for (int i = 0; i < 26; i += 2) {
            fill[i].position = sf::Vector2f(pss2[i], pss2[i + 1]);
            fill[i + 1].position = sf::Vector2f(pss2[52 - i - 2], pss2[52 - i - 1]);
            fill[i].color = sf::Color(paw_r, paw_g, paw_b, paw_a);
            fill[i + 1].color = sf::Color(paw_r, paw_g, paw_b, paw_a);
        }
        window.draw(fill, rstates);

        // drawing first arm arc
        int shad = paw_edge_a / 3;
        sf::VertexArray edge(sf::TriangleStrip, 52);
        double width = paw_arc1_width;
        sf::CircleShape circ(width / 2);
        circ.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad));
        circ.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
        window.draw(circ, rstates);
        for (int i = 0; i < 50; i += 2) {
            double vec0 = pss2[i] - pss2[i + 2];
            double vec1 = pss2[i + 1] - pss2[i + 3];
            double dist = hypot(vec0, vec1);
            edge[i].position = sf::Vector2f(pss2[i] + vec1 / dist * width / 2, pss2[i + 1] - vec0 / dist * width / 2);
            edge[i + 1].position = sf::Vector2f(pss2[i] - vec1 / dist * width / 2, pss2[i + 1] + vec0 / dist * width / 2);
            edge[i].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
            edge[i + 1].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
            width -= 0.08;
        }
        double vec0 = pss2[50] - pss2[48];
        double vec1 = pss2[51] - pss2[49];
        dist = hypot(vec0, vec1);
        edge[51].position = sf::Vector2f(pss2[50] + vec1 / dist * width / 2, pss2[51] - vec0 / dist * width / 2);
        edge[50].position = sf::Vector2f(pss2[50] - vec1 / dist * width / 2, pss2[51] + vec0 / dist * width / 2);
        edge[50].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
        edge[51].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
        window.draw(edge, rstates);
        circ.setRadius(width / 2);
        circ.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
        window.draw(circ, rstates);

        // drawing second arm arc
        sf::VertexArray edge2(sf::TriangleStrip, 52);
        width = paw_arc2_width;
        sf::CircleShape circ2(width / 2);
        circ2.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a));
        circ2.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
        window.draw(circ2, rstates);
        for (int i = 0; i < 50; i += 2) {
            vec0 = pss2[i] - pss2[i + 2];
            vec1 = pss2[i + 1] - pss2[i + 3];
            double dist = hypot(vec0, vec1);
            edge2[i].position = sf::Vector2f(pss2[i] + vec1 / dist * width / 2, pss2[i + 1] - vec0 / dist * width / 2);
            edge2[i + 1].position = sf::Vector2f(pss2[i] - vec1 / dist * width / 2, pss2[i + 1] + vec0 / dist * width / 2);
            edge2[i].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
            edge2[i + 1].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
            width -= 0.08;
        }
        vec0 = pss2[50] - pss2[48];
        vec1 = pss2[51] - pss2[49];
        dist = hypot(vec0, vec1);
        edge2[51].position = sf::Vector2f(pss2[50] + vec1 / dist * width / 2, pss2[51] - vec0 / dist * width / 2);
        edge2[50].position = sf::Vector2f(pss2[50] - vec1 / dist * width / 2, pss2[51] + vec0 / dist * width / 2);
        edge2[50].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
        edge2[51].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
        window.draw(edge2, rstates);
        circ2.setRadius(width / 2);
        circ2.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
        window.draw(circ2, rstates);


        /*
         * capture key states
         */
        // left key
        bool left_key = helpers::is_pressed(left_key_value);

        if (left_key) {
            if (!left_key_state) {
                key_state = 1;
                left_key_state = true;
            }
        } else {
            left_key_state = false;
        }


        // right key
        bool right_key = helpers::is_pressed(right_key_value);

        if (right_key) {
            if (!right_key_state) {
                key_state = 2;
                right_key_state = true;
            }
        } else {
            right_key_state = false;
        }

        // wave key
        bool wave_key = helpers::is_pressed(wave_key_value);

        if (wave_key) {
            if (!wave_key_state) {
                key_state = 3;
                wave_key_state = true;
            }
        } else {
            wave_key_state = false;
        }


        /*
         * drawing key states & other sprites
         */
        if (!left_key_state && !right_key_state && !wave_key_state) {
            key_state = 0;
            window.draw(up, rstates);
        }
        if (key_state == 1) {
            if ((clock() - std::max(timer_right_key, timer_wave_key)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(is_left_handed ? left : right, rstates);
                timer_left_key = clock();
            } else {
                window.draw(up, rstates);
            }
        } else if (key_state == 2) {
            if ((clock() - std::max(timer_left_key, timer_wave_key)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(is_left_handed ? right : left, rstates);
                timer_right_key = clock();
            } else {
                window.draw(up, rstates);
            }
        } else if (key_state == 3) {
            if ((clock() - std::max(timer_left_key, timer_right_key)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(wave, rstates);
                timer_wave_key = clock();
            } else {
                window.draw(up, rstates);
            }
        }

        // draw tablet
        if (!is_mouse) {
            window.draw(device, rstates);
        }
    }
};
