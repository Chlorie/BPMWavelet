#pragma once

#include <vector>

typedef std::vector<float> Vec;

class Wavelet
{
protected:
    Vec lowPassFilter;
    Vec highPassFilter;
    void BuildHighPassFilter(); // Build HPF from LPF
public:
    void Transform(Vec& signal, int layers); // Apply cascading DWTs to the original signal
};
