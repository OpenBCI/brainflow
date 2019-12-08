#pragma once

#include <cstdarg>
#include <string>

// include it here to allow user include only this single file
#include "board_controller.h"
#include "brainflow_constants.h"
#include "brainflow_exception.h"

/// LogLevels enum to store all possible log levels
enum class LogLevels : int
{
    LEVEL_TRACE = 0,    /// TRACE
    LEVEL_DEBUG = 1,    /// DEBUG
    LEVEL_INFO = 2,     /// INFO
    LEVEL_WARN = 3,     /// WARN
    LEVEL_ERROR = 4,    /// ERROR
    LEVEL_CRITICAL = 5, /// CRITICAL
    LEVEL_OFF = 6       // OFF
};


/// BoardShim class to communicate with a board
class BoardShim
{

    void reshape_data (int data_points, double *linear_buffer, double **output_buf);
    std::string input_params;

public:
    /// disable BrainFlow logger
    static void disable_board_logger ();
    /// enable BrainFlow logger with LEVEL_INFO
    static void enable_board_logger ();
    /// enable BrainFlow logger with LEVEL_TRACE
    static void enable_dev_board_logger ();
    /// redirect BrainFlow logger from stderr to file
    static void set_log_file (char *log_file);
    /// use set_log_level only if you want to write your own log messages to BrainFlow logger
    static void set_log_level (int log_level);
    /// write user defined string to BrainFlow logger
    static void log_message (int log_level, const char *format, ...);

    /// get sampling rate for this board, info about sampling rate is hardcoded in json file
    static int get_sampling_rate (int board_id);
    /// get row index which holds package nums
    static int get_package_num_channel (int board_id);
    /// get row index which holds timestamps
    static int get_timestamp_channel (int board_id);
    /// get number of rows in returned from @ref get_board_data() 2d array
    static int get_num_rows (int board_id);
    /// get row indices which hold EEG data, for some board we can not split EEG\EMG\...
    static int *get_eeg_channels (int board_id, int *len);
    /// get row indices which hold EMG data, for some board we can not split EEG\EMG\...
    static int *get_emg_channels (int board_id, int *len);
    /// get row indices which hold ECG data, for some board we can not split EEG\EMG\...
    static int *get_ecg_channels (int board_id, int *len);
    /// get row indices which hold EOG data, for some board we can not split EEG\EMG\...
    static int *get_eog_channels (int board_id, int *len);
    /// get row indices which hold PPG data
    static int *get_ppg_channels (int board_id, int *len);
    /// get row indices which hold EDA data
    static int *get_eda_channels (int board_id, int *len);
    /// get row indices which hold accel data
    static int *get_accel_channels (int board_id, int *len);
    /// get row indices which hold ANALOG data
    static int *get_analog_channels (int board_id, int *len);
    /// get row indices which hold gyro data
    static int *get_gyro_channels (int board_id, int *len);
    /// get row indices which hold other information for this board
    static int *get_other_channels (int board_id, int *len);

    int board_id;

    BoardShim (int board_id, struct BrainFlowInputParams params);
    ~BoardShim ()
    {
    }

    /// prepare BrainFlow's streaming session, should be called first
    void prepare_session ();
    // clang-format off
    /**
     * start streaming thread and store data in ringbuffer
     * @param buffer_size size of internal ring buffer
     * @param streamer_params use it to pass data packages further or store them directly during streaming,
                    supported values: file://%file_name%:w, file://%file_name%:a, streaming_board://%ip%:%port%
     */
    void start_stream (int buffer_size = 450000, char *streamer_params = NULL);
    // clang-format on
    /// stop streaming thread, doesnt release other resources
    void stop_stream ();
    /// release streaming session
    void release_session ();
    /// get latest collected data, doesnt remove it from ringbuffer
    double **get_current_board_data (int num_samples, int *num_data_points);
    /// get number of packages in ringbuffer
    int get_board_data_count ();
    /// get all collected data and flush it from internal buffer
    double **get_board_data (int *num_data_points);
    /// send string to a board, use it carefully and only if you understand what you are doing
    void config_board (char *config);
};
