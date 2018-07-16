#include "Wavelet.h"

void Wavelet::BuildHighPassFilter()
{
    int length = lowPassFilter.size();
    highPassFilter.resize(length);
    for (int i = 0; i < length; i++)
    {
        float value = lowPassFilter[length - i - 1];
        highPassFilter[i] = (i % 2) ? value : -value;
    }
}

void Wavelet::Transform(Vec& signal, int layers)
{
    int length = signal.size();
    for (int layer = 1; layer <= layers; layer++)
    {
        Vec temp(length);
        int waveletLength = lowPassFilter.size();

        for (int i = 0; i < length / 2; i++)
        {
            // Approximation part
            temp[i] = 0;
            for (int j = 0; j < waveletLength; j++) temp[i] += lowPassFilter[j] * signal[(j + 2 * i) % length];

            // Detail part
            temp[length / 2 + i] = 0;
            for (int j = 0; j < waveletLength; j++) temp[length / 2 + i] += highPassFilter[j] * signal[(j + 2 * i) % length];
        }

        for (int i = 0; i < length; i++) signal[i] = temp[i];
        length >>= 1;
    }
}
