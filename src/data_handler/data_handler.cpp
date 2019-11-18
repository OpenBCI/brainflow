#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "data_handler.h"
#include "downsample_operators.h"
#include "rolling_filter.h"
#include "wavelet_helpers.h"

#include "DspFilters/Dsp.h"

#include "wauxlib.h"
#include "wavelib.h"


int perform_lowpass (double *data, int data_len, int sampling_rate, double cutoff, int order,
    int filter_type, double ripple)
{
    int numSamples = 2000;
    double *filter_data[1];
    filter_data[0] = data;

    Dsp::Filter *f = NULL;
    if ((order < 1) || (order > MAX_FILTER_ORDER) || (!data))
    {
        return GENERAL_ERROR;
    }
    switch (filter_type)
    {
        case BUTTERWORTH:
            // "1024" is the number of samples over which to fade parameter changes
            f = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::LowPass<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case CHEBYSHEV_TYPE_1:
            f = new Dsp::SmoothedFilterDesign<Dsp::ChebyshevI::Design::LowPass<MAX_FILTER_ORDER>, 1,
                Dsp::DirectFormII> (1024);
            break;
        case BESSEL:
            f = new Dsp::SmoothedFilterDesign<Dsp::Bessel::Design::LowPass<MAX_FILTER_ORDER>, 1,
                Dsp::DirectFormII> (1024);
            break;
        default:
            return INVALID_ARGUMENTS_ERROR;
    }

    Dsp::Params params;
    params[0] = sampling_rate; // sample rate
    params[1] = order;         // order
    params[2] = cutoff;        // cutoff
    if (filter_type == CHEBYSHEV_TYPE_1)
    {
        params[3] = ripple; // ripple
    }
    f->setParams (params);
    f->process (data_len, filter_data);
    delete f;

    return STATUS_OK;
}

int perform_highpass (double *data, int data_len, int sampling_rate, double cutoff, int order,
    int filter_type, double ripple)
{
    Dsp::Filter *f = NULL;
    double *filter_data[1];
    filter_data[0] = data;

    if ((order < 1) || (order > MAX_FILTER_ORDER) || (!data))
    {
        return INVALID_ARGUMENTS_ERROR;
    }
    switch (filter_type)
    {
        case BUTTERWORTH:
            // "1024" is the number of samples over which to fade parameter changes
            f = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::HighPass<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case CHEBYSHEV_TYPE_1:
            f = new Dsp::SmoothedFilterDesign<Dsp::ChebyshevI::Design::HighPass<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case BESSEL:
            f = new Dsp::SmoothedFilterDesign<Dsp::Bessel::Design::HighPass<MAX_FILTER_ORDER>, 1,
                Dsp::DirectFormII> (1024);
            break;
        default:
            return INVALID_ARGUMENTS_ERROR;
    }
    Dsp::Params params;
    params[0] = sampling_rate; // sample rate
    params[1] = order;         // order
    params[2] = cutoff;        // cutoff
    if (filter_type == CHEBYSHEV_TYPE_1)
    {
        params[3] = ripple; // ripple
    }
    f->setParams (params);
    f->process (data_len, filter_data);
    delete f;

    return STATUS_OK;
}

int perform_bandpass (double *data, int data_len, int sampling_rate, double center_freq,
    double band_width, int order, int filter_type, double ripple)
{
    Dsp::Filter *f = NULL;
    double *filter_data[1];
    filter_data[0] = data;

    if ((order < 1) || (order > MAX_FILTER_ORDER) || (!data))
    {
        return INVALID_ARGUMENTS_ERROR;
    }
    switch (filter_type)
    {
        case BUTTERWORTH:
            // "1024" is the number of samples over which to fade parameter changes
            f = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::BandPass<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case CHEBYSHEV_TYPE_1:
            f = new Dsp::SmoothedFilterDesign<Dsp::ChebyshevI::Design::BandPass<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case BESSEL:
            f = new Dsp::SmoothedFilterDesign<Dsp::Bessel::Design::BandPass<MAX_FILTER_ORDER>, 1,
                Dsp::DirectFormII> (1024);
            break;
        default:
            return INVALID_ARGUMENTS_ERROR;
    }

    Dsp::Params params;
    params[0] = sampling_rate; // sample rate
    params[1] = order;         // order
    params[2] = center_freq;   // center freq
    params[3] = band_width;
    if (filter_type == CHEBYSHEV_TYPE_1)
    {
        params[4] = ripple; // ripple
    }
    f->setParams (params);

    f->process (data_len, filter_data);
    delete f;

    return STATUS_OK;
}

int perform_bandstop (double *data, int data_len, int sampling_rate, double center_freq,
    double band_width, int order, int filter_type, double ripple)
{
    Dsp::Filter *f = NULL;
    double *filter_data[1];
    filter_data[0] = data;

    if ((order < 1) || (order > MAX_FILTER_ORDER) || (!data))
    {
        return INVALID_ARGUMENTS_ERROR;
    }
    switch (filter_type)
    {
        case BUTTERWORTH:
            // "1024" is the number of samples over which to fade parameter changes
            f = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::BandStop<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case CHEBYSHEV_TYPE_1:
            f = new Dsp::SmoothedFilterDesign<Dsp::ChebyshevI::Design::BandStop<MAX_FILTER_ORDER>,
                1, Dsp::DirectFormII> (1024);
            break;
        case BESSEL:
            f = new Dsp::SmoothedFilterDesign<Dsp::Bessel::Design::BandStop<MAX_FILTER_ORDER>, 1,
                Dsp::DirectFormII> (1024);
            break;
        default:
            return INVALID_ARGUMENTS_ERROR;
    }

    Dsp::Params params;
    params[0] = sampling_rate; // sample rate
    params[1] = order;         // order
    params[2] = center_freq;   // center freq
    params[3] = band_width;
    if (filter_type == CHEBYSHEV_TYPE_1)
    {
        params[4] = ripple; // ripple
    }
    f->setParams (params);
    f->process (data_len, filter_data);
    delete f;

    return STATUS_OK;
}

int perform_rolling_filter (double *data, int data_len, int period, int agg_operation)
{
    if ((data == NULL) || (period <= 0))
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    RollingFilter<double> *filter = NULL;
    switch (agg_operation)
    {
        case MEAN:
            filter = new RollingAverage<double> (period);
            break;
        case MEDIAN:
            filter = new RollingMedian<double> (period);
            break;
        case EACH:
            return STATUS_OK;
        default:
            return INVALID_ARGUMENTS_ERROR;
    }
    for (int i = 0; i < data_len; i++)
    {
        filter->add_data (data[i]);
        data[i] = filter->get_value ();
    }
    delete filter;
    return STATUS_OK;
}

int perform_downsampling (
    double *data, int data_len, int period, int agg_operation, double *output_data)
{
    if ((data == NULL) || (data_len <= 0) || (period <= 0) || (output_data == NULL))
    {
        return INVALID_ARGUMENTS_ERROR;
    }
    double (*downsampling_op) (double *, int);
    switch (agg_operation)
    {
        case MEAN:
            downsampling_op = downsample_mean;
            break;
        case MEDIAN:
            downsampling_op = downsample_median;
            break;
        case EACH:
            downsampling_op = downsample_each;
            break;
        default:
            return INVALID_ARGUMENTS_ERROR;
    }
    int num_values = data_len / period;
    for (int i = 0; i < num_values; i++)
    {
        output_data[i] = downsampling_op (data + i * period, period);
    }
    return STATUS_OK;
}

// https://github.com/rafat/wavelib/wiki/DWT-Example-Code
int perform_wavelet_transform (
    double *data, int data_len, char *wavelet, double *output_data, int *output_len)
{
    if ((data == NULL) || (data_len <= 0) || (wavelet == NULL) || (output_data == NULL) ||
        (output_len == NULL) || (!validate_wavelet (wavelet)))
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    wave_object obj = NULL;
    wt_object wt = NULL;

    try
    {
        obj = wave_init (wavelet);
        wt = wt_init (obj, "dwt", data_len,
            1); // set decomposition level to 1, its much harder to make it configurable
        setDWTExtension (wt, "sym");
        setWTConv (wt, "direct");
        dwt (wt, data);
        *output_len = wt->outlength;
        for (int i = 0; i < *output_len; i++)
        {
            output_data[i] = wt->output[i];
        }
        wave_free (obj);
        wt_free (wt);
    }
    catch (const std::exception &e)
    {
        if (obj)
        {
            wave_free (obj);
        }
        if (wt)
        {
            wt_free (wt);
        }
        // more likely exception here occured because input buffer is to small to perform wavelet
        // transform
        return INVALID_BUFFER_SIZE_ERROR;
    }
    return STATUS_OK;
}

// inside wavelib inverse transform uses internal state from direct transform, dirty hack to restore
// it here
int perform_inverse_wavelet_transform (double *wavelet_coeffs, int coeffs_len,
    int original_data_len, char *wavelet, double *output_data)
{
    if ((wavelet_coeffs == NULL) || (coeffs_len <= 0) || (original_data_len <= 0) ||
        (wavelet == NULL) || (output_data == NULL) || (coeffs_len == original_data_len) ||
        (!validate_wavelet (wavelet)))
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    wave_object obj;
    wt_object wt;

    try
    {
        obj = wave_init (wavelet);
        wt = wt_init (obj, "dwt", original_data_len, 1);
        setDWTExtension (wt, "sym");
        setWTConv (wt, "direct");
        wt->length[0] = coeffs_len / 2;
        wt->length[1] = coeffs_len / 2;
        for (int i = 0; i < coeffs_len; i++)
        {
            wt->output[i] = wavelet_coeffs[i];
        }
        idwt (wt, output_data);
        wave_free (obj);
        wt_free (wt);
    }
    catch (const std::exception &e)
    {
        if (obj)
        {
            wave_free (obj);
        }
        if (wt)
        {
            wt_free (wt);
        }
        // more likely exception here occured because input buffer is to small to perform wavelet
        // transform
        return INVALID_BUFFER_SIZE_ERROR;
    }
    return STATUS_OK;
}

int perform_wavelet_denoising (double *data, int data_len, char *wavelet, int decomposition_level)
{
    if ((data == NULL) || (data_len <= 0) || (decomposition_level <= 0) ||
        (!validate_wavelet (wavelet)))
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    denoise_object obj;
    double *temp = new double[data_len];
    try
    {
        obj = denoise_init (data_len, decomposition_level, wavelet);
        setDenoiseMethod (obj, "visushrink");
        setDenoiseWTMethod (obj, "dwt");
        setDenoiseWTExtension (obj, "sym");
        setDenoiseParameters (obj, "soft", "all");
        denoise (obj, data, temp);
        for (int i = 0; i < data_len; i++)
        {
            data[i] = temp[i];
        }
        delete[] temp;
    }
    catch (const std::exception &e)
    {
        delete[] temp;
        if (obj)
        {
            denoise_free (obj);
        }
        // more likely exception here occured because input buffer is to small to perform wavelet
        // transform
        return INVALID_BUFFER_SIZE_ERROR;
    }
    return STATUS_OK;
}

int write_file (double *data, int num_rows, int num_cols, char *file_name, char *file_mode)
{
    if ((strcmp (file_mode, "w") != 0) && (strcmp (file_mode, "w+") != 0) &&
        (strcmp (file_mode, "a") != 0) && (strcmp (file_mode, "a+") != 0))
    {
        return INVALID_ARGUMENTS_ERROR;
    }
    FILE *fp;
    fp = fopen (file_name, file_mode);
    if (fp == NULL)
    {
        return GENERAL_ERROR;
    }

    // in read/write file data is transposed!
    for (int i = 0; i < num_cols; i++)
    {
        for (int j = 0; j < num_rows - 1; j++)
        {
            fprintf (fp, "%lf,", data[j * num_cols + i]);
        }
        fprintf (fp, "%lf\n", data[(num_rows - 1) * num_cols + i]);
    }
    fclose (fp);
    return STATUS_OK;
}

int read_file (double *data, int *num_rows, int *num_cols, char *file_name, int num_elements)
{
    if (num_elements <= 0)
    {
        return INVALID_ARGUMENTS_ERROR;
    }
    FILE *fp;
    fp = fopen (file_name, "r");
    if (fp == NULL)
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    char buf[4096];
    // rows and cols in csv file, in data array its transposed!
    int total_rows = 0;
    int total_cols = 0;

    // count rows
    char c;
    for (c = getc (fp); c != EOF; c = getc (fp))
    {
        if (c == '\n')
        {
            total_rows++;
        }
    }

    fseek (fp, 0, SEEK_SET);
    int current_row = 0;
    int cur_pos = 0;
    while (fgets (buf, sizeof (buf), fp) != NULL)
    {
        std::string csv_string (buf);
        std::stringstream ss (csv_string);
        std::vector<std::string> splitted;
        std::string tmp;
        while (getline (ss, tmp, ','))
        {
            splitted.push_back (tmp);
        }
        total_cols = splitted.size ();
        for (int i = 0; i < total_cols; i++)
        {
            data[i * total_rows + current_row] = std::stod (splitted[i]);
            cur_pos++;
            if (cur_pos == (num_elements - 1))
            {
                *num_cols = current_row + 1;
                *num_rows = total_cols;
                fclose (fp);
                return STATUS_OK;
            }
        }
        current_row++;
    }
    // more likely code below is unreachable
    *num_cols = total_rows;
    *num_rows = total_cols;
    fclose (fp);
    return STATUS_OK;
}

int get_num_elements_in_file (char *file_name, int *num_elements)
{
    FILE *fp;
    fp = fopen (file_name, "r");
    if (fp == NULL)
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    char buf[4096];
    int total_rows = 0;

    // count rows
    char c;
    for (c = getc (fp); c != EOF; c = getc (fp))
    {
        if (c == '\n')
        {
            total_rows++;
        }
    }
    if (total_rows == 0)
    {
        *num_elements = 0;
        fclose (fp);
        return EMPTY_BUFFER_ERROR;
    }

    fseek (fp, 0, SEEK_SET);
    while (fgets (buf, sizeof (buf), fp) != NULL)
    {
        std::string csv_string (buf);
        std::stringstream ss (csv_string);
        std::vector<std::string> splitted;
        std::string tmp;
        while (getline (ss, tmp, ','))
        {
            splitted.push_back (tmp);
        }
        *num_elements = splitted.size () * total_rows;
        fclose (fp);
        return STATUS_OK;
    }
    *num_elements = 0;
    fclose (fp);
    return EMPTY_BUFFER_ERROR;
}
