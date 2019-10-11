#include <stdlib.h>
#include <string.h>

#include "board_controller.h"
#include "board_shim.h"

#define MAX_CHANNELS 256

/////////////////////////////////////////
//////////// logging methods ////////////
/////////////////////////////////////////

void BoardShim::enable_board_logger ()
{
    int res = set_log_level (2);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to enable board logger", res);
    }
}

void BoardShim::disable_board_logger ()
{
    int res = set_log_level (6);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to disable board logger", res);
    }
}

void BoardShim::enable_dev_board_logger ()
{
    int res = set_log_level (0);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to enable dev board logger", res);
    }
}

void BoardShim::set_log_file (char *log_file)
{
    int res = ::set_log_file (log_file);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to set log file", res);
    }
}

//////////////////////////////////////////
/////// data acquisition methods /////////
//////////////////////////////////////////

BoardShim::BoardShim (int board_id, const char *port_name)
{
    strcpy (this->port_name, port_name);
    this->board_id = board_id;
}

void BoardShim::prepare_session ()
{
    int res = ::prepare_session (board_id, port_name);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to prepare session", res);
    }
}

void BoardShim::start_stream (int buffer_size)
{
    int res = ::start_stream (buffer_size, board_id, port_name);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to start stream", res);
    }
}

void BoardShim::stop_stream ()
{
    int res = ::stop_stream (board_id, port_name);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to stop stream", res);
    }
}

void BoardShim::release_session ()
{
    int res = ::release_session (board_id, port_name);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to release session", res);
    }
}

int BoardShim::get_board_data_count ()
{
    int data_count = 0;
    int res = ::get_board_data_count (&data_count, board_id, port_name);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to get board data count", res);
    }
    return data_count;
}

double **BoardShim::get_board_data (int *num_data_points)
{
    int num_samples = get_board_data_count ();
    int num_data_channels = get_num_rows (board_id);
    double *buf = new double[num_samples * num_data_channels];
    int res = ::get_board_data (num_samples, buf, board_id, port_name);
    if (res != STATUS_OK)
    {
        delete[] buf;
        throw BrainFlowException ("failed to get board data", res);
    }

    double **output_buf = new double *[num_data_channels];
    for (int i = 0; i < num_data_channels; i++)
    {
        output_buf[i] = new double[num_samples];
    }
    reshape_data (num_samples, buf, output_buf);
    delete[] buf;
    *num_data_points = num_samples;
    return output_buf;
}

double **BoardShim::get_current_board_data (int num_samples, int *num_data_points)
{
    int num_data_channels = BoardShim::get_num_rows (board_id);
    double *buf = new double[num_samples * num_data_channels];
    int res = ::get_current_board_data (num_samples, buf, num_data_points, board_id, port_name);
    if (res != STATUS_OK)
    {
        delete[] buf;
        throw BrainFlowException ("failed to get board data", res);
    }

    double **output_buf = new double *[num_data_channels];
    for (int i = 0; i < num_data_channels; i++)
    {
        output_buf[i] = new double[*num_data_points];
    }
    reshape_data (*num_data_points, buf, output_buf);
    delete[] buf;

    return output_buf;
}

void BoardShim::config_board (char *config)
{
    int res = ::config_board (config, board_id, port_name);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed to config board", res);
    }
}

// for better user experience and consistency accross bindings we return 2d array from user api, we
// can not do it directly in low level api because some languages can not pass multidim array to C++
void BoardShim::reshape_data (int num_data_points, double *linear_buffer, double **output_buf)
{
    int num_data_channels = BoardShim::get_num_rows (board_id);
    for (int i = 0; i < num_data_channels; i++)
    {
        memcpy (
            output_buf[i], linear_buffer + i * num_data_points, sizeof (double) * num_data_points);
    }
}

//////////////////////////////////////////
///////////// data desc methods //////////
//////////////////////////////////////////

int BoardShim::get_sampling_rate (int board_id)
{
    int sampling_rate = -1;
    int res = ::get_sampling_rate (board_id, &sampling_rate);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return sampling_rate;
}

int BoardShim::get_package_num_channel (int board_id)
{
    int package_num_channel = -1;
    int res = ::get_package_num_channel (board_id, &package_num_channel);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return package_num_channel;
}

int BoardShim::get_timestamp_channel (int board_id)
{
    int timestamp_channel = 0;
    int res = ::get_timestamp_channel (board_id, &timestamp_channel);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return timestamp_channel;
}

int BoardShim::get_num_rows (int board_id)
{
    int num_rows = 0;
    int res = ::get_num_rows (board_id, &num_rows);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return num_rows;
}

int *BoardShim::get_eeg_channels (int board_id, int *len)
{
    int *eeg_channels = new int[MAX_CHANNELS];
    int res = ::get_eeg_channels (board_id, eeg_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return eeg_channels;
}

int *BoardShim::get_emg_channels (int board_id, int *len)
{
    int *emg_channels = new int[MAX_CHANNELS];
    int res = ::get_emg_channels (board_id, emg_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return emg_channels;
}

int *BoardShim::get_ecg_channels (int board_id, int *len)
{
    int *ecg_channels = new int[MAX_CHANNELS];
    int res = ::get_ecg_channels (board_id, ecg_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return ecg_channels;
}

int *BoardShim::get_eog_channels (int board_id, int *len)
{
    int *eog_channels = new int[MAX_CHANNELS];
    int res = ::get_eog_channels (board_id, eog_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return eog_channels;
}

int *BoardShim::get_eda_channels (int board_id, int *len)
{
    int *eda_channels = new int[MAX_CHANNELS];
    int res = ::get_eda_channels (board_id, eda_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return eda_channels;
}

int *BoardShim::get_ppg_channels (int board_id, int *len)
{
    int *ppg_channels = new int[MAX_CHANNELS];
    int res = ::get_ppg_channels (board_id, ppg_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return ppg_channels;
}

int *BoardShim::get_accel_channels (int board_id, int *len)
{
    int *accel_channels = new int[MAX_CHANNELS];
    int res = ::get_accel_channels (board_id, accel_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return accel_channels;
}

int *BoardShim::get_other_channels (int board_id, int *len)
{
    int *other_channels = new int[MAX_CHANNELS];
    int res = ::get_other_channels (board_id, other_channels, len);
    if (res != STATUS_OK)
    {
        throw BrainFlowException ("failed get board info", res);
    }
    return other_channels;
}