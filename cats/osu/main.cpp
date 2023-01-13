#include "header.hpp"
#include <iostream>

namespace osu {
Json::Value left_key_value, right_key_value, smoke_key_value, wave_key_value;
int offset_x, offset_y;
int x_paw_start, y_paw_start, x_paw_end, y_paw_end;
int paw_r, paw_g, paw_b, paw_a;
int paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a;
double scale;
bool is_mouse, is_left_handed, is_enable_toggle_smoke;
sf::Sprite bg, up, left, right, device, smoke, wave;

int key_state = 0;

bool use_any_key = false;
bool left_key_state = false;
bool right_key_state = false;
bool wave_key_state = false;
bool previous_smoke_key_state = false;
bool current_smoke_key_state = false;
bool is_toggle_smoke = false;
double timer_left_key = -1;
double timer_right_key = -1;
double timer_wave_key = -1;

bool init() {
    /*
     * load config
     */
    Json::Value osu = data::cfg["cats"]["osu"];

    is_mouse = osu["mouse"].asBool();
    is_enable_toggle_smoke = osu["toggleSmoke"].asBool();

    use_any_key = osu["useAnyKey"].asBool();

    x_paw_start = osu["pawStartingPoint"][0].asInt();
    y_paw_start = osu["pawStartingPoint"][1].asInt();
    x_paw_end = osu["pawEndingPoint"][0].asInt();
    y_paw_end = osu["pawEndingPoint"][1].asInt();

    paw_r = osu["pawColor"][0].asInt();
    paw_g = osu["pawColor"][1].asInt();
    paw_b = osu["pawColor"][2].asInt();
    paw_a = osu["pawColor"].size() == 3 ? 255 : osu["pawColor"][3].asInt();

    paw_edge_r = osu["pawEdgeColor"][0].asInt();
    paw_edge_g = osu["pawEdgeColor"][1].asInt();
    paw_edge_b = osu["pawEdgeColor"][2].asInt();
    paw_edge_a = osu["pawEdgeColor"].size() == 3 ? 255 : osu["pawEdgeColor"][3].asInt();

    bool chk[256];
    std::fill(chk, chk + 256, false);

    /* key1 & key2 */
    if (!use_any_key) {
        left_key_value = osu["key1"];
        for (Json::Value &v : left_key_value) {
            chk[v.asInt()] = true;
        }

        right_key_value = osu["key2"];
        for (Json::Value &v : right_key_value) {
            if (chk[v.asInt()]) {
                data::error_msg("Overlapping osu! keybinds", "Error reading configs");
                return false;
            }
        }
    }

    /* wave */
    wave_key_value = osu["wave"];
    for (Json::Value &v : wave_key_value) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osu! keybinds", "Error reading configs");
            return false;
        }
    }

    /* smoke */
    smoke_key_value = osu["smoke"];
    for (Json::Value &v : smoke_key_value) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osu! keybinds", "Error reading configs");
            return false;
        }
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
    // TODO: Load sprites relitivly to avoid needing to hard code cats/{catname}/img/{file}
    up.setTexture(data::load_texture("cats/osu/img/up.png"));
    left.setTexture(data::load_texture("cats/osu/img/left.png"));
    right.setTexture(data::load_texture("cats/osu/img/right.png"));
    wave.setTexture(data::load_texture("cats/osu/img/wave.png"));
    if (is_mouse) {
        bg.setTexture(data::load_texture("cats/osu/img/mousebg.png"));
        device.setTexture(data::load_texture("cats/osu/img/mouse.png"), true);
    } else {
        bg.setTexture(data::load_texture("cats/osu/img/tabletbg.png"));
        device.setTexture(data::load_texture("cats/osu/img/tablet.png"), true);
    }
    smoke.setTexture(data::load_texture("cats/osu/img/smoke.png"));
    device.setScale(scale, scale);

    return true;
}

void draw(const sf::RenderStates& rstates) {
    window.draw(bg);

    /* 
     * initializing pss and pss2 (kuvster's magic)
     */
    auto [x, y] = input::get_xy();
    y += data::cfg["windowHeight"].asInt() - BASE_HEIGHT;
    int oof = 6;
    std::vector<double> pss = {(float) x_paw_start, (float) y_paw_start};
    double dist = hypot(x_paw_start - x, y_paw_start - y);
    double centreleft0 = x_paw_start - 0.7237 * dist / 2;
    double centreleft1 = y_paw_start + 0.69 * dist / 2;
    for (int i = 1; i < oof; i++) {
        std::vector<double> bez = {(float) x_paw_start, (float) y_paw_start, centreleft0, centreleft1, x, y};
        auto [p0, p1] = input::bezier(1.0 * i / oof, bez, 6);
        pss.push_back(p0);
        pss.push_back(p1);
    }
    pss.push_back(x);
    pss.push_back(y);
    double a = y - centreleft1;
    double b = centreleft0 - x;
    double le = hypot(a, b);
    a = x + a / le * 60;
    b = y + b / le * 60;
    dist = hypot(x_paw_end - a, y_paw_end - b);
    double centreright0 = x_paw_end - 0.6 * dist / 2;
    double centreright1 = y_paw_end + 0.8 * dist / 2;
    int push = 20;
    double s = x - centreleft0;
    double t = y - centreleft1;
    le = hypot(s, t);
    s *= push / le;
    t *= push / le;
    double s2 = a - centreright0;
    double t2 = b - centreright1;
    le = hypot(s2, t2);
    s2 *= push / le;
    t2 *= push / le;
    for (int i = 1; i < oof; i++) {
        std::vector<double> bez = {x, y, x + s, y + t, a + s2, b + t2, a, b};
        auto [p0, p1] = input::bezier(1.0 * i / oof, bez, 8);
        pss.push_back(p0);
        pss.push_back(p1);
    }
    pss.push_back(a);
    pss.push_back(b);
    for (int i = oof - 1; i > 0; i--) {
        std::vector<double> bez = {1.0 * x_paw_end, 1.0 * y_paw_end, centreright0, centreright1, a, b};
        auto [p0, p1] = input::bezier(1.0 * i / oof, bez, 6);
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
        auto [p0, p1] = input::bezier(1.0 * i / iter, pss, 38);
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

    // draw smoke
    bool is_smoke_key_pressed = false;
    for (Json::Value &v : smoke_key_value) {
        if (input::is_pressed(v.asInt())) {
            is_smoke_key_pressed = true;
            break;
        }
    }

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
    window.draw(fill);

    // drawing first arm arc
    int shad = paw_edge_a / 3;
    sf::VertexArray edge(sf::TriangleStrip, 52);
    double width = 7;
    sf::CircleShape circ(width / 2);
    circ.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad));
    circ.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
    window.draw(circ);
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
    window.draw(edge);
    circ.setRadius(width / 2);
    circ.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
    window.draw(circ);

    // drawing second arm arc
    sf::VertexArray edge2(sf::TriangleStrip, 52);
    width = 6;
    sf::CircleShape circ2(width / 2);
    circ2.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a));
    circ2.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
    window.draw(circ2);
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
    window.draw(edge2);
    circ2.setRadius(width / 2);
    circ2.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
    window.draw(circ2);


    /*
     * capture key states
     */
    if (!use_any_key) {
        // left key
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


        // right key
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
    }

    // wave key
    bool wave_key = false;
    for (Json::Value &v : wave_key_value) {
        if (input::is_pressed(v.asInt())) {
            wave_key = true;
            break;
        }
    }

    if (wave_key) {
        if (!wave_key_state) {
            key_state = 3;
            wave_key_state = true;
        }
    } else {
        wave_key_state = false;
    }


    /*
     * drawing keypresses & other sprites
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
}; // namespace osu
