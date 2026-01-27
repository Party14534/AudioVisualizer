#ifndef WAV_H
#define WAV_H

#include "../audio.h"

typedef unsigned char byte;

struct WavHeader {
    // RIFF Chunk
    byte FileTypeBlocID[4]; // Must be <<RIFF>> (4)
    byte FileSize[4]; // File size minus 8 bytes (4)
    byte FileFormatID[4]; // Must be <<WAVE>> (4)
    
    // Data format
    byte FormatBlocID[4]; // Must be <<fmt␣>> (4)
    byte BlocSize[4]; // Chunk size minus 8 bytes (4)
    byte AudioFormat[2]; // (1: PCM Integer, 3: IEEE 754 float) (2)
    byte NbrChannels[2]; // Number of channels (2)
    byte Frequency[4]; // Sample rate in hertz (4)
    byte BytePerSec[4]; // Number of bytes to read per second (Frequency * BytePerBloc) (4)
    byte BytePerBloc[2]; // Number of bytes per block (NbrChannels * BitsPerSample / 8) (2)
    byte BitsPerSample[2]; // Number of bits per sample (2)

    // Chunk containing the sampled data
    byte DataBlocID[4]; // Must be <<data>> (4)
    byte DataSize[4]; // Sampled Data size (4)

    i16* PCMData;
    float* IEEEData;
};

struct AudioFile* getWav(std::string pathToFile);

#endif
