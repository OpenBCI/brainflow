#pragma once

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

    const int port = 2390;
    void read_thread ();

public:
    NovaXR (char *ip_addr);
    ~NovaXR ();

    int prepare_session ();
    int start_stream (int buffer_size);
    int stop_stream ();
    int release_session ();
    int get_board_id ()
    {
        return NOVAXR_BOARD;
    }
};
