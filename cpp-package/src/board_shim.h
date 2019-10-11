#pragma once

// include it here to allow user include only this single file
#include "brainflow_constants.h"
#include "brainflow_exception.h"

class BoardShim
{

    void reshape_data (int data_points, double *linear_buffer, double **output_buf);

public:
    // logging methods
    static void disable_board_logger ();
    static void enable_board_logger ();
    static void enable_dev_board_logger ();
    static void set_log_file (char *log_file);

    // data desc and board desc methods, these methods return column indexes in data table
    static int get_sampling_rate (int board_id);
    static int get_package_num_channel (int board_id);
    static int get_timestamp_channel (int board_id);
    static int get_num_rows (int board_id);
    static int *get_eeg_channels (int board_id, int *len);
    static int *get_emg_channels (int board_id, int *len);
    static int *get_ecg_channels (int board_id, int *len);
    static int *get_eog_channels (int board_id, int *len);
    static int *get_ppg_channels (int board_id, int *len);
    static int *get_eda_channels (int board_id, int *len);
    static int *get_accel_channels (int board_id, int *len);
    static int *get_gyro_channels (int board_id, int *len);
    static int *get_other_channels (int board_id, int *len);

    int board_id;
    char port_name[1024];

    BoardShim (int board_id, const char *port_name);
    ~BoardShim ()
    {
    }

    // data acquisition methods
    void prepare_session ();
    void start_stream (int buffer_size = 450000);
    void stop_stream ();
    void release_session ();
    double **get_current_board_data (int num_samples, int *num_data_points);
    int get_board_data_count ();
    double **get_board_data (int *num_data_points);
    void config_board (char *config);
};
