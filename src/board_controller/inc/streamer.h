#pragma once


class Streamer
{
public:
    Streamer ()
    {
    }
    virtual ~Streamer ()
    {
    }

    virtual int init_streamer () = 0;
    virtual void stream_data (double *data, int size, double timestamp) = 0;
};
