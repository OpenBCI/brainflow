#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

#include "board.h"
#include "board_controller.h"
#include "socket.h"

class NovaXR : public Board
{

private:
    volatile bool keep_alive;
    bool initialized;
    bool is_streaming;
    std::thread streaming_thread;
    int num_channels;
    Socket socket;

    std::mutex m;
    std::condition_variable cv;
    volatile int state;
    void read_thread ();

public:
    NovaXR (char *ip_addr);
    ~NovaXR ();

    int prepare_session ();
    int start_stream (int buffer_size);
    int stop_stream ();
    int release_session ();
    int config_board (char *config);
};
