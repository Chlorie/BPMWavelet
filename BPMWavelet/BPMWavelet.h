#pragma once

#include "Wavelet.h"

class BPMWavelet
{
private:
    Wavelet * wavelet;
    float* samples;
    int channels;
    int sampleCount;
    int sampleRate;
    int layers;
    Vec windowBPMs;
    // Autocorrelation function, return value in BPM
    float Autocorrelation(const Vec& sum, float maxValue, float minValue, bool integral); 
public:
    // Constructor
    // [samples] A float array containing all the samples
    // [channels] Channel amount of this clip
    // [sampleCount] Sample amount of all channels
    // [sampleRate] Sample rate of the audio file in Hz
    // [wavelet] A pointer to a Wavelet object
    BPMWavelet(float* samples, int channels, int sampleCount, int sampleRate, Wavelet* wavelet) : samples(samples), channels(channels), sampleCount(sampleCount), sampleRate(sampleRate), wavelet(wavelet) {}
    // Calculate BPM
    // [layers] DWT layer
    // [windowSize] Window size in samples, must be power of 2
    // [minValue] Minimum value of BPM
    // [maxValue] Maximum value of BPM
    // [integralValue] The value of BPM is an integer
    void Calculate(int layers = 4, int windowSize = 131072, float minValue = 120, float maxValue = 240, bool integralValue = true);
    float GetResult();
};
