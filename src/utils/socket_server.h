#pragma once

#include <thread>

#include "socket_client.h"


class SocketServer
{

public:
    SocketServer (const char *local_ip, int local_port);
    ~SocketServer ()
    {
        close ();
    }

    int bind ();
    int accept ();
    int recv (void *data, int size);
    void close ();
    volatile bool
        client_connected; // idea - stop accept blocking call by calling close in another thread

private:
    char local_ip[80];
    int local_port;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    std::thread accept_thread;

#ifdef _WIN32
    SOCKET server_socket;
    volatile SOCKET connected_socket;
#else
    int server_socket;
    volatile int connected_socket;
#endif
};
