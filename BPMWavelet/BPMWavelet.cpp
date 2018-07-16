#include <iostream>
#include <algorithm>

#include "BPMWavelet.h"

#define DEBUG_OUTPUT // Define this to output more info when processing the file

void BPMWavelet::Calculate(int layers, int windowSize, float minValue, float maxValue, bool integralValue)
{
    int length = sampleCount / channels; // Length in samples
    int position = 1; // The index of the currently processing windows
    Vec window(windowSize);
    this->layers = layers;

    // Process every window
    while (position * windowSize <= length)
    {
        // Process every channel
        for (int channel = 0; channel < channels; channel++)
        {
            // Apply DWT to the samples
            for (int i = 0; i < windowSize; i++) window[i] = samples[((position - 1) * windowSize + i) * channels + channel];
            wavelet->Transform(window, layers);

            for (int i = 0; i < windowSize; i++) window[i] = abs(window[i]); // Full wave rectification

            int minSize = windowSize >> layers;
            Vec sum(minSize);
            int detailLength = windowSize;

            // For the detail calculated in each layer
            for (int layer = 1; layer <= layers; layer++)
            {
                float average = 0;
                detailLength >>= 1;
                int rate = 1 << (layers - layer);

                // Downsampling
                float sample;
                for (int i = 0; i < minSize; i++)
                {
                    sample = window[detailLength + i * rate];
                    average += sample;
                    sum[i] += sample;
                }

                // Normalization
                average /= minSize;
                for (int i = 0; i < minSize; i++) sum[i] -= average;
            }

            // Final approximation should also be added into the sum
            float average = 0;
            for (int i = 0; i < minSize; i++)
            {
                average += window[i];
                sum[i] += window[i];
            }
            average /= minSize;
            for (int i = 0; i < minSize; i++) sum[i] -= average;

            float bpm = Autocorrelation(sum, maxValue, minValue, integralValue);
            windowBPMs.push_back(bpm); // Add BPM of the current window and the current channel to the list
        }
        position++;
    }
}

float BPMWavelet::Autocorrelation(const Vec& sum, float maxValue, float minValue, bool integral)
{
    int length = sum.size();
    float newSampleRate = sampleRate / (float)(1 << layers);
    float max = 0;
    float bpm = 0;
    if (integral)
    {
        for (int i = minValue; i <= maxValue; i++)
        {
            int period = 60 * newSampleRate / i;
            float average = 0;
            for (int j = 0; j < length - period; j++) average += sum[j] * sum[j + period];
            average /= length - period;
            average = abs(average);
            if (average > max)
            {
                bpm = i;
                max = average;
            }
        }
    }
    else
    {
        int shortest = (int)(60 * newSampleRate / maxValue);
        int longest = (int)(60 * newSampleRate / minValue);
        for (int period = shortest; period <= longest; period++)
        {
            float average = 0;
            for (int i = 0; i < length - period; i++) average += sum[i] * sum[i + period];
            average /= length - period;
            average = abs(average);
            if (average > max)
            {
                bpm = 60.0 * newSampleRate / period;
                max = average;
            }
        }
    }
    return bpm;
}

float BPMWavelet::GetResult()
{
    int size = windowBPMs.size();
    if (size == 0) throw std::exception("Please call BPMWavelet::Calculate before trying to get the result");
    std::sort(windowBPMs.begin(), windowBPMs.end());
    return windowBPMs[size / 2]; // Get median of the list
}
