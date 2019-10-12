#pragma once

#ifdef _WIN32
#define SHARED_EXPORT __declspec(dllexport)
#define CALLING_CONVENTION __cdecl
#else
#define SHARED_EXPORT
#define CALLING_CONVENTION
#endif

#include "board_info_getter.h"
#include "brainflow_constants.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // data acquisition methods
    SHARED_EXPORT int CALLING_CONVENTION prepare_session (int board_id, char *port_name);
    SHARED_EXPORT int CALLING_CONVENTION start_stream (
        int buffer_size, int board_id, char *port_name);
    SHARED_EXPORT int CALLING_CONVENTION stop_stream (int board_id, char *port_name);
    SHARED_EXPORT int CALLING_CONVENTION release_session (int board_id, char *port_name);
    SHARED_EXPORT int get_current_board_data (
        int num_samples, double *data_buf, int *returned_samples, int board_id, char *port_name);
    SHARED_EXPORT int CALLING_CONVENTION get_board_data_count (
        int *result, int board_id, char *port_name);
    SHARED_EXPORT int CALLING_CONVENTION get_board_data (
        int data_count, double *data_buf, int board_id, char *port_name);
    SHARED_EXPORT int CALLING_CONVENTION config_board (char *config, int board_id, char *port_name);

    // logging methods
    SHARED_EXPORT int CALLING_CONVENTION set_log_level (int log_level);
    SHARED_EXPORT int CALLING_CONVENTION set_log_file (char *log_file);

#ifdef __cplusplus
}
#endif