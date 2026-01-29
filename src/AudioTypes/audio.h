#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <fftw3.h>
#include <string>
#include <cstdlib>
#include <stdio.h>

#define kiss_fft_scalar int16_t
#include <kiss_fft.h>
#include <kiss_fftr.h>

// Undefine to avoid conflicts with other code
#undef kiss_fft_scalar

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
    std::vector<i16> fft_input;
    std::vector<float> band_edges = {0.0f, 0.05f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f};

    kiss_fftr_cfg fft_cfg;
    size_t current_fft_size;

    AudioFile();
    //AudioFile(const i16* _samples, u64 _sampleCount, u32 _channelCount, u32 _sampleRate);
    
    void buildInput();
    std::vector<float> processFrame(const std::vector<i16>& frame);
};


AudioFile* getWav(std::string pathToFile);

#endif
