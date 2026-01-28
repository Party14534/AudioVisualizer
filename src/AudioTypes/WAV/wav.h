#ifndef WAV_H
#define WAV_H

#include "../audio.h"

typedef unsigned char byte;

struct WavHeader {
    // RIFF Chunk
    byte FileTypeBlocID[4]; // Must be <<RIFF>> (4)
    u32 FileSize; // File size minus 8 bytes (4)
    byte FileFormatID[4]; // Must be <<WAVE>> (4)
    
    // Data format
    byte FormatBlocID[4]; // Must be <<fmt␣>> (4)
    u32 BlocSize; // Chunk size minus 8 bytes (4)
    byte AudioFormat[2]; // (1: PCM Integer, 3: IEEE 754 float) (2)
    u16 NbrChannels; // Number of channels (2)
    u32 Frequency; // Sample rate in hertz (4)
    u32 BytePerSec; // Number of bytes to read per second (Frequency * BytePerBloc) (4)
    byte BytePerBloc[2]; // Number of bytes per block (NbrChannels * BitsPerSample / 8) (2)
    byte BitsPerSample[2]; // Number of bits per sample (2)

    // Chunk containing the sampled data
    byte DataBlocID[4]; // Must be <<data>> (4)
    u32 DataSize; // Sampled Data size (4)

    i16* PCMData;
    float* IEEEData;
};

struct AudioFile* getWav(std::string pathToFile);

#endif
