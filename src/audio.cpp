#include <audio.hpp>
#include <global.hpp>

//============================================================================
// LOAD FROM ...
//============================================================================
sf::Sound *BongoAudio::loadSoundFromFile(const std::string &path) {
    sf::SoundBuffer *buffer = new sf::SoundBuffer();

    if (!buffer->loadFromFile(path)) {
        return nullptr;
    }

    sf::Sound *sound = new sf::Sound();
    sound->setBuffer(*buffer);

    return sound;
}

sf::Music *BongoAudio::loadMusicFromFile(const std::string &path) {
    sf::Music *music = new sf::Music();

    if (!music->openFromFile(path)) {
        return nullptr;
    }

    return music;
}

//============================================================================
// BIND TO LUA
//============================================================================
void BongoAudio::bindToLua() {
    luabridge::getGlobalNamespace(LuaState)
        .beginNamespace("BongoAudio")
        // load functions
        .addFunction("loadSoundFromFile", &BongoAudio::loadSoundFromFile)
        .addFunction("loadMusicFromFile", &BongoAudio::loadMusicFromFile)
        // all things like controlling playback is defined in sfml.hpp
        .endNamespace();
}
