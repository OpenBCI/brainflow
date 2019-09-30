#pragma once

#include "socket_client.h"


class SocketServer
{

public:
    SocketServer (const char *local_ip, const char *client_ip, int local_port, int client_port);
    ~SocketServer ()
    {
        close ();
    }

    int bind ();
    int recv (void *data, int size);
    void close ();

private:
    char local_ip[80];
    char client_ip[80];
    int client_port;
    int local_port;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
#ifdef _WIN32
    SOCKET server_socket;
#else
    int server_socket;
#endif
};
