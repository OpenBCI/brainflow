#pragma once

#ifdef _WIN32
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT
#endif

#include "board_info_getter.h"
#include "brainflow_constants.h"

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

#ifdef __cplusplus
}
#endif