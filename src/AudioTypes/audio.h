#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <string>
#include <cstdlib>
#include <stdio.h>

typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::int16_t i16;
typedef std::int32_t i32;

struct AudioFile {
    sf::SoundBuffer buffer;
    u32 Frequency;
    u32 channelCount;
    u64 sampleCount;

    AudioFile(const i16* _samples, u64 _sampleCount, u32 _channelCount, u32 _sampleRate);
};

AudioFile* getWav(std::string pathToFile);

#endif
