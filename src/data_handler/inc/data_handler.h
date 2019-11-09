#pragma once

#ifdef _WIN32
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT
#endif

#include "brainflow_constants.h"

#define MAX_FILTER_ORDER 8 // define it here to show in the docs

typedef enum name
{
    BUTTERWORTH = 0,
    CHEBYSHEV_TYPE_1 = 1,
    BESSEL = 2
} FilterTypes;


#ifdef __cplusplus
extern "C"
{
#endif
    // signal processing methods
    // ripple param uses only for chebyshev filter
    SHARED_EXPORT int perform_lowpass (double *data, int data_len, int sampling_rate, double cutoff,
        int order, int filter_type, double ripple);
    SHARED_EXPORT int perform_highpass (double *data, int data_len, int sampling_rate,
        double cutoff, int order, int filter_type, double ripple);
    SHARED_EXPORT int perform_bandpass (double *data, int data_len, int sampling_rate,
        double center_freq, double band_width, int order, int filter_type, double ripple);
    SHARED_EXPORT int perform_bandstop (double *data, int data_len, int sampling_rate,
        double center_freq, double band_width, int order, int filter_type, double ripple);
    // file operations
    SHARED_EXPORT int write_file (
        double *data, int num_rows, int num_cols, char *file_name, char *file_mode);
    SHARED_EXPORT int read_file (
        double *data, int *num_rows, int *num_cols, char *file_name, int max_elements);
#ifdef __cplusplus
}
#endif
