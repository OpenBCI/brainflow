#pragma once

#ifdef _WIN32
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT
#endif

#include "brainflow_constants.h"

typedef enum name
{
    BUTTERWORTH = 0,
    CHEBYSHEV = 1,
    BESSEL = 2
} FilterTypes;


#ifdef __cplusplus
extern "C"
{
#endif
    // ripple param uses only for chebyshev filter, max filter order is 7
    int perform_lowpass (double *data, int data_len, int sampling_rate, double cutoff, int order,
        int filter_type, double ripple);
    int perform_highpass (double *data, int data_len, int sampling_rate, double cutoff, int order,
        int filter_type, double ripple);
    int perform_bandpass (double *data, int data_len, int sampling_rate, double center_freq,
        double band_width, int order, int filter_type, double ripple);
    int perform_bandstop (double *data, int data_len, int sampling_rate, double center_freq,
        double band_width, int order, int filter_type, double ripple);
#ifdef __cplusplus
}
#endif
