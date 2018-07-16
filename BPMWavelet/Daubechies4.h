#pragma once

#include "Wavelet.h"

class Daubechies4 : public Wavelet
{
public:
    Daubechies4()
    {
        lowPassFilter.resize(8);
        lowPassFilter[0] = -0.010597401784997278;
        lowPassFilter[1] = 0.032883011666982945;
        lowPassFilter[2] = 0.030841381835986965;
        lowPassFilter[3] = -0.18703481171888114;
        lowPassFilter[4] = -0.02798376941698385;
        lowPassFilter[5] = 0.6308807679295904;
        lowPassFilter[6] = 0.7148465705525415;
        lowPassFilter[7] = 0.23037781330885523;
        BuildHighPassFilter();
    }
};
