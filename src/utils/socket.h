#pragma once

#ifdef _WIN32
#include <ws2tcpip.h>
// should be included before windows.h
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>


enum class SocketReturnCodes
{
    STATUS_OK = 0,
    WSA_STARTUP_ERROR = 1,
    CREATE_SOCKET_ERROR = 2,
    CONNECT_ERROR = 3
};

class Socket
{

public:
    Socket (const char *port_name, int port);
    ~Socket ()
    {
        close ();
    }

    int connect ();
    int send (void *data, int size);
    int recv (void *data, int size);
    void close ();
    char *get_ip_addr ()
    {
        return ip_addr;
    }
    int get_port ()
    {
        return port;
    }

private:
    char ip_addr[32];
    int port;
#ifdef _WIN32
    SOCKET connect_socket;
#else

#endif
};
