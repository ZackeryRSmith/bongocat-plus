#ifndef BONGO_CAT_AUDIO_HPP
#define BONGO_CAT_AUDIO_HPP
#include <global.hpp>

namespace BongoAudio {
//============================================================================
// LOAD FROM ...
//============================================================================
sf::Sound *loadSoundFromFile(const std::string &);
sf::Music *loadMusicFromFile(const std::string &);
//============================================================================
// BIND TO LUA
//============================================================================
void bindToLua();
}; // namespace BongoAudio
#endif
