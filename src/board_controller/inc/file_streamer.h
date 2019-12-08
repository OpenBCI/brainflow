#pragma once

#include <stdio.h>

#include "streamer.h"


class FileStreamer : public Streamer
{

public:
    FileStreamer (const char *file, const char *file_mode);
    ~FileStreamer ();

    int init_streamer ();
    void stream_data (double *data, int size, double timestamp);

private:
    char file[128];
    char file_mode[128];
    FILE *fp;
};
