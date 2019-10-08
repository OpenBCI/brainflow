#pragma once

#ifdef _WIN32
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT
#endif

typedef enum
{
    STATUS_OK = 0,
    PORT_ALREADY_OPEN_ERROR,
    UNABLE_TO_OPEN_PORT_ERROR,
    SET_PORT_ERROR,
    BOARD_WRITE_ERROR,
    INCOMMING_MSG_ERROR,
    INITIAL_MSG_ERROR,
    BOARD_NOT_READY_ERROR,
    STREAM_ALREADY_RUN_ERROR,
    INVALID_BUFFER_SIZE_ERROR,
    STREAM_THREAD_ERROR,
    STREAM_THREAD_IS_NOT_RUNNING,
    EMPTY_BUFFER_ERROR,
    INVALID_ARGUMENTS_ERROR,
    UNSUPPORTED_BOARD_ERROR,
    BOARD_NOT_CREATED_ERROR,
    ANOTHER_BOARD_IS_CREATED_ERROR,
    GENERAL_ERROR,
    SYNC_TIMEOUT_ERROR
} CustomExitCodes;

typedef enum
{
    SYNTHETIC_BOARD = -1,
    CYTON_BOARD = 0,
    GANGLION_BOARD = 1,
    CYTON_DAISY_BOARD = 2,
    NOVAXR_BOARD = 3,
    GANGLION_WIFI_BOARD = 4,
    CYTON_WIFI_BOARD = 5,
    CYTON_DAISY_WIFI_BOARD = 6
} BoardIds;

#ifdef __cplusplus
extern "C"
{
#endif
    // data acquisition methods
    SHARED_EXPORT int prepare_session (int board_id, char *port_name);
    SHARED_EXPORT int start_stream (int buffer_size, int board_id, char *port_name);
    SHARED_EXPORT int stop_stream (int board_id, char *port_name);
    SHARED_EXPORT int release_session (int board_id, char *port_name);
    SHARED_EXPORT int get_current_board_data (
        int num_samples, double *data_buf, int *returned_samples, int board_id, char *port_name);
    SHARED_EXPORT int get_board_data_count (int *result, int board_id, char *port_name);
    SHARED_EXPORT int get_board_data (
        int data_count, double *data_buf, int board_id, char *port_name);
    SHARED_EXPORT int config_board (char *config, int board_id, char *port_name);

    // logging methods
    SHARED_EXPORT int set_log_level (int log_level);
    SHARED_EXPORT int set_log_file (char *log_file);

    // data desc and board desc methods, these methods return column indexes in data table returned
    // by get_board_data
    SHARED_EXPORT int get_sampling_rate (int board_id, int *sampling_rate);
    SHARED_EXPORT int get_package_num_channel (int board_id, int *package_num_channel);
    SHARED_EXPORT int get_timestamp_channel (int board_id, int *timestamp_channel);
    SHARED_EXPORT int get_num_rows (int board_id, int *num_rows);
    SHARED_EXPORT int get_eeg_channels (int board_id, int *eeg_channels, int *len);
    SHARED_EXPORT int get_emg_channels (int board_id, int *emg_channels, int *len);
    SHARED_EXPORT int get_ecg_channels (int board_id, int *ecg_channels, int *len);
    SHARED_EXPORT int get_eog_channels (int board_id, int *eog_channels, int *len);
    SHARED_EXPORT int get_ppg_channels (int board_id, int *ppg_channels, int *len);
    SHARED_EXPORT int get_eda_channels (int board_id, int *eda_channels, int *len);
    SHARED_EXPORT int get_accel_channels (int board_id, int *accel_channels, int *len);
    SHARED_EXPORT int get_gyro_channels (int board_id, int *gyro_channels, int *len);
    SHARED_EXPORT int get_other_channels (int board_id, int *other_channels, int *len);

#ifdef __cplusplus
}
#endif