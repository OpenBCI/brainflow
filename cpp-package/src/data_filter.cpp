#include <string.h>

#include "data_filter.h"
#include "data_handler.h"

void DataFilter::perform_lowpass (double *data, int data_len, int sampling_rate, double cutoff,
    int order, int filter_type, double ripple)
{
    int res = ::perform_lowpass (data, data_len, sampling_rate, cutoff, order, filter_type, ripple);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to filter signal", res);
    }
}

void DataFilter::perform_highpass (double *data, int data_len, int sampling_rate, double cutoff,
    int order, int filter_type, double ripple)
{
    int res =
        ::perform_highpass (data, data_len, sampling_rate, cutoff, order, filter_type, ripple);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to filter signal", res);
    }
}

void DataFilter::perform_bandpass (double *data, int data_len, int sampling_rate,
    double center_freq, double band_width, int order, int filter_type, double ripple)
{
    int res = ::perform_bandpass (
        data, data_len, sampling_rate, center_freq, band_width, order, filter_type, ripple);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to filter signal", res);
    }
}

void DataFilter::perform_bandstop (double *data, int data_len, int sampling_rate,
    double center_freq, double band_width, int order, int filter_type, double ripple)
{
    int res = ::perform_bandstop (
        data, data_len, sampling_rate, center_freq, band_width, order, filter_type, ripple);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to filter signal", res);
    }
}

double **DataFilter::read_file (int *num_rows, int *num_cols, char *file_name, int max_elements)
{
    double *data_linear = new double[max_elements];
    int res = ::read_file (data_linear, num_rows, num_cols, file_name, max_elements);
    if (res != STATUS_OK)
    {
        delete[] data_linear;
        throw BrainFlowException ("failed to read file", res);
    }
    double **output_buf = new double *[*num_rows];
    for (int i = 0; i < *num_rows; i++)
    {
        output_buf[i] = new double[*num_cols];
    }

    DataFilter::reshape_data_to_2d (*num_rows, *num_cols, data_linear, output_buf);
    delete[] data_linear;

    return output_buf;
}

void DataFilter::write_file (
    double **data, int num_rows, int num_cols, char *file_name, char *file_mode)
{
    double *data_linear = new double[num_rows * num_cols];
    DataFilter::reshape_data_to_1d (num_rows, num_cols, data, data_linear);
    int res = ::write_file (data_linear, num_rows, num_cols, file_name, file_mode);
    if (res != STATUS_OK)
    {
        delete[] data_linear;
        throw BrainFlowException ("failed to write file", res);
    }
    delete[] data_linear;
}

void DataFilter::reshape_data_to_1d (int num_rows, int num_cols, double **buf, double *output_buf)
{
    for (int i = 0; i < num_cols; i++)
    {
        for (int j = 0; j < num_rows; j++)
        {
            output_buf[j * num_cols + i] = buf[j][i];
        }
    }
}

void DataFilter::reshape_data_to_2d (
    int num_rows, int num_cols, double *linear_buffer, double **output_buf)
{
    for (int i = 0; i < num_rows; i++)
    {
        memcpy (output_buf[i], linear_buffer + i * num_cols, sizeof (double) * num_rows);
    }
}
