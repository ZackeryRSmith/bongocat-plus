#include "header.hpp"
#include <memory>
#define BONGO_ERROR 1

#if defined(__unix__) || defined(__unix)
#include <unistd.h>
#include <limits.h>

extern "C" {
#include <SDL2/SDL.h>
}
#else
#include <windows.h>
#endif

/*
 * INCLUDE CUSTOM CAT LOGIC HERE
 */
#include "custom/Osu.cpp"
#include "custom/OsuTaiko.cpp"
#include "custom/OsuCatch.cpp"
#include "custom/OsuMania.cpp"
//#include "custom/CatInHat.cpp"


/*
 * ADD CUSTOM CAT CLASSES HERE!
 */
// TODO: Make switch case relate to config.json
//       so the order of "cats" changes the code up here
class CatFactory {
public:
    static std::shared_ptr<Cat> create(int i) {
        std::cout << i << std::endl;
        fflush(stdout);
        switch (i) {
            case 1: return std::make_shared<Osu>();
                    break;
            case 2: return std::make_shared<OsuTaiko>();
                    break;
            case 3: return std::make_shared<OsuCatch>();
                    break;
            case 4: return std::make_shared<OsuMania>();
                    break;
            //case 5: return std::make_shared<CatInHat>();
            //        break;
            default:
                throw std::invalid_argument("Could not find that cat");
        }
    }
};

const char *default_conf_string = 
R"V0G0N({
})V0G0N";

namespace data {
Json::Value cfg;
std::map<std::string, sf::Texture> img_holder;

void create_config() {
    std::string error;
    Json::CharReaderBuilder cfg_builder;
    Json::CharReader *cfg_reader = cfg_builder.newCharReader();
    cfg_reader->parse(default_conf_string, default_conf_string + strlen(default_conf_string), &cfg, &error);
    delete cfg_reader;
}

void error_msg(std::string error, std::string title) {
#if defined(__unix__) || defined(__unix)

    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Retry" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Cancel" },
    };

    SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255, 255, 255 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            { 0, 0, 0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 0, 0, 0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            { 255, 255, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 128, 128, 128 }
        }
    };

    SDL_MessageBoxData messagebox_data = {
    	SDL_MESSAGEBOX_ERROR,
    	NULL,
    	title.c_str(),
    	error.c_str(),
    	SDL_arraysize(buttons),
    	buttons,
    	&colorScheme
    };

    int button_id;

    SDL_ShowMessageBox(&messagebox_data, &button_id);

    if (button_id == -1 || button_id == 1) {
#else
    if (MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_RETRYCANCEL) == IDCANCEL) {
#endif
        exit(BONGO_ERROR);
    }
}

bool update(Json::Value &cfg_default, Json::Value &cfg) {
    bool is_update = true;
    for (const auto &key : cfg.getMemberNames()) {
        if (cfg_default.isMember(key)) {
            if (cfg_default[key].type() != cfg[key].type()) {
                error_msg("Value type error in config.json", "Error reading configs");
                return false;
            }
            if (cfg_default[key].isArray() && !cfg_default[key].empty()) {
                for (Json::Value &v : cfg[key]) {
                    if (v.type() != cfg_default[key][0].type()) {
                        error_msg("Value type in array error in config.json", "Error reading configs");
                        return false;
                    }
                }
            }
            if (cfg_default[key].isObject()) {
                is_update &= update(cfg_default[key], cfg[key]);
            } else {
                cfg_default[key] = cfg[key];
            }
        } else {
            cfg_default[key] = cfg[key];
        }
    }
    return is_update;
}

std::shared_ptr<Cat> init() {
    while (true) {
        create_config();
        std::ifstream cfg_file("config.json", std::ifstream::binary);
        if (!cfg_file.good()) {
            break;
        }
        std::string cfg_string((std::istreambuf_iterator<char>(cfg_file)), std::istreambuf_iterator<char>()), error;
        Json::CharReaderBuilder cfg_builder;
        Json::CharReader *cfg_reader = cfg_builder.newCharReader();
        Json::Value cfg_read;
        if (!cfg_reader->parse(cfg_string.c_str(), cfg_string.c_str() + cfg_string.size(), &cfg_read, &error)) {
            delete cfg_reader;
            error_msg("Syntax error in config.json:\n" + error, "Error reading configs");
        } else if (update(cfg, cfg_read)) {
            delete cfg_reader;
            break;
        }
    }

    img_holder.clear();

    int cat_choice = data::cfg["cat"].asInt();

    std::shared_ptr<Cat> cat;
    try {
        cat = CatFactory::create(cat_choice);
    }
    catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }

    return cat;
}

sf::Texture &load_texture(std::string path) {
    if (img_holder.find(path) == img_holder.end()) {
        while (!img_holder[path].loadFromFile(path)) {
            error_msg("Cannot find file " + path, "Error importing images");
        }
    }
    return img_holder[path];
}
}; // namespace data
