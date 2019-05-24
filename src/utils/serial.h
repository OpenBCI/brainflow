#ifndef SERIAL
#define SERIAL

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>


class Serial
{

public:
    Serial (const char *port_name);
    ~Serial ()
    {
        close_serial_port ();
    }

    int open_serial_port ();
    bool is_port_open ();
    int set_serial_port_settings ();
    int read_from_serial_port (void *b, int size);
    int send_to_serial_port (const void *message);
    int close_serial_port ();
    char *get_port_name ()
    {
        return port_name;
    }

private:
    char port_name[1024];
#ifdef _WIN32
    HANDLE port_descriptor;
#else
    int port_descriptor;
#endif
};

#endif
