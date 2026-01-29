#include "audio.h"
#include "kiss_fftr.h"
#include <cmath>
#include <cstdint>
#include <iostream>

AudioFile::AudioFile() {}

void AudioFile::buildInput() {
    const i16* buff = buffer.getSamples();

    size_t channelCount = buffer.getChannelCount();
    size_t sampleCount = buffer.getSampleCount();

    size_t inLen = sampleCount / channelCount;
    fft_input = std::vector<i16>(inLen);

    for (int i = 0; i < sampleCount; i+=channelCount) {
        i16 val = 0;
        for (int j = 0; j < channelCount; j++) {
            val += buff[i+j];
        }

        val /= channelCount;
        fft_input[i/channelCount] = val; 
    }
}

std::vector<float> AudioFile::processFrame(const std::vector<i16>& frame) {
    size_t frame_size = frame.size();
    if (!fft_cfg || current_fft_size != frame_size) {
        // Free existing configuration if it exists
        if (fft_cfg) {
            kiss_fftr_free(fft_cfg);
            fft_cfg = nullptr;
        }
        
        // Create new configuration
        fft_cfg = kiss_fftr_alloc(frame_size, 0, nullptr, nullptr);
        if (!fft_cfg) {
            std::cerr << "Failed to allocate FFT configuration!" << std::endl;
            return {};
        }
        
        current_fft_size = frame_size;
        std::cout << "Created FFT config for size: " << frame_size << std::endl;
    }

    if (frame_size < 2) {
        return {};
    }

    printf("Gonna perform fft\n");
    // Perform FFT
    size_t nBins = frame.size() / 2 + 1;
    kiss_fft_cpx* fftOutput = new kiss_fft_cpx[nBins];
    kiss_fftr(fft_cfg, frame.data(), fftOutput);

    const float maxPowerR = 1.f / ((float)INT16_MAX * (float)INT16_MAX);
    std::vector<float> spectrum = std::vector<float>(nBins);
    for (int i = 0; i < nBins; i++) {
        float real = static_cast<float>(fftOutput[i].r);
        float imag = static_cast<float>(fftOutput[i].i);

        float power = real * real + imag * imag;

        spectrum[i] = 10.0f * log10f(power * maxPowerR + 1e-10f);
    }
    printf("built spectrum\n");

    delete[] fftOutput;

    std::vector<float> bands(band_edges.size() - 1, 0.0f);
        
    float bin_width = 1.0f;  // Normalized frequency per bin
    
    for (size_t bin = 0; bin < spectrum.size(); ++bin) {
        float freq = static_cast<float>(bin) / spectrum.size();
        
        // Find which band this frequency belongs to
        for (size_t band = 0; band < bands.size(); ++band) {
            if (freq >= band_edges[band] && freq < band_edges[band + 1]) {
                // Convert from dB back to linear for averaging
                float linear_value = powf(10.0f, spectrum[bin] / 10.0f);
                bands[band] += linear_value;
                break;
            }
        }
    }
    
    // Convert back to dB and average
    for (size_t band = 0; band < bands.size(); ++band) {
        if (bands[band] > 0) {
            bands[band] = 10.0f * log10f(bands[band]);
        }
    }

    return bands;  
}
