#pragma once

#include "data_buffer.h"
#include "spdlog/spdlog.h"

#define MAX_CAPTURE_SAMPLES (86400 * 250) // should be enough for one day of capturing


class Board
{
public:
    static spdlog::logger *board_logger;
    static int set_log_level (int level);

    virtual ~Board ()
    {
        if (db != NULL)
        {
            delete db;
            db = NULL;
        }
    }
    Board ()
    {
        skip_logs = false;
        db = NULL; // should be initialized in start_stream
    }
    virtual int prepare_session () = 0;
    virtual int start_stream (int buffer_size) = 0;
    virtual int stop_stream () = 0;
    virtual int release_session () = 0;
    virtual int config_board (char *config) = 0;

    int get_current_board_data (
        int num_samples, float *data_buf, double *ts_buf, int *returned_samples);
    int get_board_data_count (int *result);
    int get_board_data (int data_count, float *data_buf, double *ts_buf);

protected:
    DataBuffer *db;
    bool skip_logs; // we can not use spdlog in destructor call, but we need to call release_session
                    // and stop_stream from destructor, idea - set this variable to true in
                    // destructor and write logs under this check where resultuired
};
