#pragma once

#include <thread>

#include "board.h"
#include "board_controller.h"

#ifdef _WIN32
#include "cdevice.h"
#endif

class BrainBit : public Board
{

private:
    // as for now BrainBit supports only windows, to dont write ifdef in board_controller.cpp and in
    // CmakeLists.txt add dummy implementation for Unix
#ifdef _WIN32
    volatile bool keep_alive;
    bool initialized;
    bool is_streaming;
    std::thread streaming_thread;
    Device *device;
    void read_thread ();

    int find_device (long long serial_number);
    int find_device_info (
        DeviceEnumerator *enumerator, uint64_t serial_number, DeviceInfo *out_device_info);
    int connect_device ();
#endif

public:
    BrainBit (struct BrainFlowInputParams params);
    ~BrainBit ();

    int prepare_session ();
    int start_stream (int buffer_size, char *streamer_params);
    int stop_stream ();
    int release_session ();
    int config_board (char *config);
};
