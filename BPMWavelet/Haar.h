#pragma once

#include "Wavelet.h"

class Haar : public Wavelet
{
private:
    const float oneOverSqrt2 = 1 / sqrt(2.0);
public:
    Haar()
    {
        lowPassFilter.resize(2);
        lowPassFilter[0] = oneOverSqrt2;
        lowPassFilter[1] = oneOverSqrt2;
        BuildHighPassFilter();
    }
};
