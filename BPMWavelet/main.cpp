#include <iostream>

// Wave file decoder from "https://mackron.github.io/dr_wav"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

// BPM calculator
#include "BPMWavelet.h"
#include "Haar.h"
#include "Daubechies4.h"

void error_exit(const char* str)
{
    std::cerr << str << std::endl;
    std::cin.get();
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 2) error_exit("Invalid argument(s)\nUsage: BPMWavelet [wav file path]");

    // Read audio file
    drwav* decoder = drwav_open_file(argv[1]);
    if (decoder == nullptr) error_exit("Failed to open audio file");
    int sampleCount = decoder->totalSampleCount;
    int channels = decoder->channels;
    int sampleRate = decoder->sampleRate;
    float* samples = new float[sampleCount];
    uint64_t readSample = drwav_read_f32(decoder, sampleCount, samples);
    drwav_close(decoder);

    std::cout << "Audio file: " << argv[1] << std::endl;
    std::cout << "Sample count: " << sampleCount << std::endl;
    std::cout << "Channel count: " << channels << std::endl;
    std::cout << "Sample rate: " << sampleRate << std::endl << std::endl;

    // Calculate BPM
    Wavelet* wavelet = new Daubechies4(); // Switch this out to try other wavelets for DWT
    BPMWavelet bpmCalc(samples, channels, sampleCount, sampleRate, wavelet); // Construct a BPM calculator
    bpmCalc.Calculate(); // Calculate
    float bpm = bpmCalc.GetResult(); // Get the result
    std::cout << std::endl << "Calculated BPM: " << bpm << std::endl;

    std::cin.get();
    return 0;
}
