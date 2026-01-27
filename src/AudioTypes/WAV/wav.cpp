#include "wav.h"
#include <cstdio>

AudioFile* getWav(std::string pathToFile) {
    WavHeader h;
    AudioFile* audioFile = (AudioFile*)malloc(sizeof(AudioFile));
    
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

    u16 format = (h.AudioFormat[1] << 8) | h.AudioFormat[0];

    printf("%d\n", format);

    fclose(f);
    return audioFile;
}
