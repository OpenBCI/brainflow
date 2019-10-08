#pragma once

#include "brainflow_exception.h"

class DataHandler
{
public:
    DataHandler (int board_id);

    void filter_lowpass (
        double **data, int *channels, int channels_size, int data_count, float cutoff);
    void filter_highpass (
        double **data, int *channels, int channels_size, int data_count, float cutoff);
    void filter_bandpass (double **data, int *channels, int channels_size, int data_count,
        float min_cutoff, float max_cutoff);
    void remove_dc_offset (
        double **data, int *channels, int channels_size, int data_count, float value = 1.0);
    void preprocess_data (double **data, int *channels, int channels_size, int data_count,
        float min_cutoff = 1.0, float max_cutoff = 50.0, float dc_offset = 1.0);

private:
    int board_id;
};
