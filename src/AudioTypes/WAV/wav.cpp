#include "wav.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include <cstdio>

AudioFile* getWav(std::string pathToFile) {
    WavHeader h;
    
    FILE* f = fopen(pathToFile.c_str(), "r");
    
    // Read RIFF Chunk
    fread(&h.FileTypeBlocID, 4, 1, f);
    fread(&h.FileSize, 4, 1, f);
    fread(&h.FileFormatID, 4, 1, f);

    // Validate RIFF
    if (h.FileTypeBlocID[0] != 'R' &&
            h.FileTypeBlocID[1] != 'I' &&
            h.FileTypeBlocID[2] != 'F' &&
            h.FileTypeBlocID[3] != 'F') {
        printf("Invalid WAV file\nError in FileTypeBlocID\n");
        exit(1);
    }

    // Validate Format
    if (h.FileFormatID[0] != 'W' &&
            h.FileFormatID[1] != 'A' &&
            h.FileFormatID[2] != 'V' &&
            h.FileFormatID[3] != 'E') {
        printf("Invalid WAV file\nError in FileFormatID\n");
        exit(1);
    }

    // Read Data format
    fread(&h.FormatBlocID, 4, 1, f);
    fread(&h.BlocSize, 4, 1, f);
    fread(&h.AudioFormat, 2, 1, f);
    fread(&h.NbrChannels, 2, 1, f);
    fread(&h.Frequency, 4, 1, f);
    fread(&h.BytePerSec, 4, 1, f);
    fread(&h.BytePerBloc, 2, 1, f);
    fread(&h.BitsPerSample, 2, 1, f);

    // Validate FormatBlocID
    if (h.FormatBlocID[0] != 'f' &&
            h.FormatBlocID[1] != 'm' &&
            h.FormatBlocID[2] != 't' &&
            h.FormatBlocID[3] != ' ') {
        printf("Invalid WAV file\nError in FormatBlocID\n");
        exit(1);
    }

    // Read sampled data
    char dataId[5] = "data";
    int dataIndex = 0;
    long dataStart;
    for(;;) {
        // TODO: do something with metadata
        byte character;
        size_t bytesRead = fread(&character, 1, 1, f);
        if(bytesRead != 1) {
            printf("Invalid WAV file\nError in DataBlocID\n");
            exit(1);
        }
        
        // Validate Data Bloc ID
        if (character == dataId[dataIndex]) {
            if (character == 'd') { dataStart = ftell(f); }
            dataIndex++;

            if (dataIndex == 4) { break; }
        }
    }

    fread(&h.DataSize, 4, 1, f);

    u16 format = (h.AudioFormat[1] << 8) | h.AudioFormat[0];
    /*u32 dataSize = (h.DataSize[0]) | (h.DataSize[1] << 8) |
        (h.DataSize[2] << 16) | (h.DataSize[3] << 24);*/

    switch (format) {
        case 0x01:
            h.PCMData = (i16*)malloc(h.DataSize);
            // TODO: make this faster
            for (int i = 0; i < h.DataSize; i+=2) {
                byte lsb, msb;
                fread(&lsb, 1, 1, f);
                fread(&msb, 1, 1, f);
                h.PCMData[i/2] = (msb << 8) | lsb;
            }
            break;
        case 0x03:
            printf("Unsupported WAVE Format\n");
            exit(1);
            break;
        default:
            printf("Unknown WAVE Format\n");
            exit(1);
            break;
    }

    // Create appropriate channel map
    std::vector<sf::SoundChannel> channelMap;

    printf("%d\n", h.NbrChannels);

    if (h.NbrChannels == 1) {
        channelMap = {sf::SoundChannel::Mono};
    } else if (h.NbrChannels == 2) {
        // Standard stereo: Left then Right
        channelMap = {sf::SoundChannel::SideLeft, sf::SoundChannel::SideRight};
    } else if (h.NbrChannels == 4) {
        // Quadraphonic (less common)
        channelMap = {
            sf::SoundChannel::FrontLeft,
            sf::SoundChannel::FrontRight,
            sf::SoundChannel::BackLeft,
            sf::SoundChannel::BackRight
        };
    } else if (h.NbrChannels == 6) {
        // 5.1 surround
        channelMap = {
            sf::SoundChannel::FrontLeft,
            sf::SoundChannel::FrontRight,
            sf::SoundChannel::FrontCenter,
            sf::SoundChannel::LowFrequencyEffects,
            sf::SoundChannel::SideLeft,
            sf::SoundChannel::SideRight
        };
    } else {
        // Fallback: assign channels sequentially
        for (unsigned int i = 0; i < h.NbrChannels; ++i) {
            channelMap.push_back(static_cast<sf::SoundChannel>(i));
        }
    }

    AudioFile* audioFile = new AudioFile; 

    // Set up AudioFile
    audioFile->buffer = sf::SoundBuffer(h.PCMData,
            h.DataSize / sizeof(i16),
            h.NbrChannels,
            h.Frequency,
            channelMap);
    audioFile->Frequency = h.Frequency;
    audioFile->channelCount = h.NbrChannels;
    audioFile->sampleCount = h.DataSize / sizeof(i16);
    printf("Channel stuff\n");

    fclose(f);
    return audioFile;
}
