#include <chrono>
#include <string.h>

#include "custom_cast.h"
#include "novaxr.h"

NovaXR::NovaXR (char *ip_addr) : Board (), socket (ip_addr, 2390, (int)SocketType::UDP)
{
    this->is_streaming = false;
    this->keep_alive = false;
    this->initialized = false;
    this->num_channels = 20;
    this->state = SYNC_TIMEOUT_ERROR;
    Board::board_logger->debug ("use ip addr {}", ip_addr);
}

NovaXR::~NovaXR ()
{
    release_session ();
}

int NovaXR::prepare_session ()
{
    if (initialized)
    {
        Board::board_logger->info ("Session is already prepared");
        return STATUS_OK;
    }
    int res = socket.connect ();
    if (res != 0)
    {
        Board::board_logger->error ("failed to init socket: {}", res);
        return GENERAL_ERROR;
    }
    initialized = true;
    return STATUS_OK;
}

int NovaXR::start_stream (int buffer_size)
{
    if (is_streaming)
    {
        Board::board_logger->error ("Streaming thread already running");
        return STREAM_ALREADY_RUN_ERROR;
    }
    if (buffer_size <= 0 || buffer_size > MAX_CAPTURE_SAMPLES)
    {
        Board::board_logger->error ("invalid array size");
        return INVALID_BUFFER_SIZE_ERROR;
    }

    if (db)
    {
        delete db;
        db = NULL;
    }

    // start streaming
    if (socket.send ("b", 1) != 1)
    {
        Board::board_logger->error ("Failed to send a command to board");
        return BOARD_WRITE_ERROR;
    }

    db = new DataBuffer (num_channels, buffer_size);
    if (!db->is_ready ())
    {
        Board::board_logger->error ("unable to prepare buffer");
        return INVALID_BUFFER_SIZE_ERROR;
    }

    keep_alive = true;
    streaming_thread = std::thread ([this] { this->read_thread (); });
    // wait for data to ensure that everything is okay
    std::unique_lock<std::mutex> lk (this->m);
    auto sec = std::chrono::seconds (1);
    if (cv.wait_for (lk, 5 * sec, [this] { return this->state != SYNC_TIMEOUT_ERROR; }))
    {
        this->is_streaming = true;
        return this->state;
    }
    else
    {
        Board::board_logger->error ("no data received in 5sec, stopping thread");
        this->is_streaming = true;
        this->stop_stream ();
        return UNABLE_TO_OPEN_PORT_ERROR;
    }
}

int NovaXR::stop_stream ()
{
    if (is_streaming)
    {
        keep_alive = false;
        is_streaming = false;
        streaming_thread.join ();
        this->state = SYNC_TIMEOUT_ERROR;
        if (socket.send ("s", 1) != 1)
        {
            Board::board_logger->error ("Failed to send a command to board");
            return BOARD_WRITE_ERROR;
        }
        return STATUS_OK;
    }
    else
    {
        return STREAM_THREAD_IS_NOT_RUNNING;
    }
}

int NovaXR::release_session ()
{
    if (initialized)
    {
        if (is_streaming)
            stop_stream ();
        initialized = false;
        socket.close ();
    }
    return STATUS_OK;
}

void NovaXR::read_thread ()
{
    int res;
    unsigned char b[63];
    while (keep_alive)
    {
        // 63 bytes in dummy package
        res = socket.recv (b, 63);
        if (res != 63)
        {
            Board::board_logger->trace ("unable to read 63 bytes, read {}", res);
            continue;
        }
        else
        {
            // inform main thread that everything is ok and first package was received
            if (this->state != STATUS_OK)
            {
                {
                    std::lock_guard<std::mutex> lk (this->m);
                    this->state = STATUS_OK;
                }
                this->cv.notify_one ();
                Board::board_logger->debug ("start streaming");
            }
        }

        float package[20];
        // package num
        package[0] = (float)b[0];
        // eeg and emg
        for (int i = 0; i < 16; i++)
        {
            package[i + 1] = (float)cast_24bit_to_int32 (b + 1 + 3 * i);
        }
        package[17] = cast_16bit_to_int32 (b + 50);
        package[18] = cast_16bit_to_int32 (b + 52);
        package[19] = cast_16bit_to_int32 (b + 54);

        // temp, I dont know how many bytes will be in real timestamp just read rand 3 bytes for now
        double timestamp = (double)cast_24bit_to_int32 (b + 55);
        db->add_data (timestamp, package);
    }
}
