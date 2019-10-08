#include <math.h>
#include <stdlib.h>

#include "board_controller.h"
#include "board_shim.h"
#include "data_handler.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

DataHandler::DataHandler (int board_id)
{
    this->board_id = board_id;
}

void DataHandler::filter_lowpass (
    double **data, int *channels, int channels_size, int data_count, float cutoff)
{
    int sampling_rate = BoardShim::get_sampling_rate (board_id);
    double rc = 1.0 / (cutoff * 2 * M_PI);
    double dt = 1.0 / sampling_rate;
    double alpha = dt / (rc + dt);
    double *temp_arr = new double[data_count];

    for (int i = 0; i < channels_size; i++)
    {
        int channel_num = channels[i];
        temp_arr[0] = data[channel_num][0];
        for (int j = 1; j < data_count; j++)
        {
            temp_arr[j] = temp_arr[j - 1] + alpha * (data[channel_num][j] - temp_arr[j - 1]);
        }
        for (int j = 0; j < data_count; j++)
        {
            data[channel_num][j] = temp_arr[j];
        }
    }
    delete[] temp_arr;
}

void DataHandler::filter_highpass (
    double **data, int *channels, int channels_size, int data_count, float cutoff)
{

    int sampling_rate = BoardShim::get_sampling_rate (board_id);
    double rc = 1.0 / (cutoff * 2 * M_PI);
    double dt = 1.0 / sampling_rate;
    double alpha = dt / (rc + dt);
    double *temp_arr = new double[data_count];

    for (int i = 0; i < channels_size; i++)
    {
        int channel_num = channels[i];
        temp_arr[0] = data[channel_num][0];
        for (int j = 1; j < data_count; j++)
        {
            temp_arr[j] =
                alpha * (temp_arr[j - 1] + data[channel_num][j] - data[channel_num][j - 1]);
        }
        for (int j = 0; j < data_count; j++)
        {
            data[channel_num][j] = temp_arr[j];
        }
    }
    delete[] temp_arr;
}

void DataHandler::filter_bandpass (double **data, int *channels, int channels_size, int data_count,
    float min_cutoff, float max_cutoff)
{
    filter_lowpass (data, channels, channels_size, data_count, max_cutoff);
    filter_highpass (data, channels, channels_size, data_count, min_cutoff);
}

void DataHandler::remove_dc_offset (
    double **data, int *channels, int channels_size, int data_count, float value)
{
    filter_highpass (data, channels, channels_size, data_count, value);
}

void DataHandler::preprocess_data (double **data, int *channels, int channels_size, int data_count,
    float min_cutoff, float max_cutoff, float dc_offset)
{
    remove_dc_offset (data, channels, channels_size, data_count, dc_offset);
    filter_bandpass (data, channels, channels_size, data_count, min_cutoff, max_cutoff);
}
