#include "socket.h"


///////////////////////////////
/////////// WINDOWS ///////////
//////////////////////////////
#ifdef _WIN32

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

Socket::Socket (const char *ip_addr, int port)
{
    strcpy (this->ip_addr, ip_addr);
    this->port = port;
    connect_socket = INVALID_SOCKET;
    memset (&socket_addr, 0, sizeof (socket_addr));
}

int Socket::connect ()
{
    WSADATA wsadata;
    int res = WSAStartup (MAKEWORD (2, 2), &wsadata);
    if (res != 0)
    {
        return (int)SocketReturnCodes::WSA_STARTUP_ERROR;
    }
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons (port);
    if (inet_pton (AF_INET, ip_addr, &socket_addr.sin_addr) == 0)
    {
        return (int)SocketReturnCodes::PTON_ERROR;
    }

    connect_socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (connect_socket == INVALID_SOCKET)
    {
        return (int)SocketReturnCodes::CREATE_SOCKET_ERROR;
    }

    // ensure that library will not hang in blocking recv/send call
    DWORD timeout = 3000;
    setsockopt (connect_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof (timeout));
    setsockopt (connect_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof (timeout));

    return (int)SocketReturnCodes::STATUS_OK;
}

int Socket::send (const char *data, int size)
{
    int len = sizeof (socket_addr);
    int res = sendto (connect_socket, data, size, 0, (sockaddr *)&socket_addr, len);
    if (res == SOCKET_ERROR)
    {
        return -1;
    }
    return res;
}

int Socket::recv (void *data, int size)
{
    int len = sizeof (socket_addr);
    int res = recvfrom (connect_socket, (char *)data, size, 0, (sockaddr *)&socket_addr, &len);
    if (res == SOCKET_ERROR)
    {
        return -1;
    }
    return res;
}

void Socket::close ()
{
    closesocket (connect_socket);
    connect_socket = INVALID_SOCKET;
    WSACleanup ();
}

///////////////////////////////
//////////// UNIX /////////////
///////////////////////////////
#else

Socket::Socket (const char *ip_addr, int port)
{
    strcpy (this->ip_addr, ip_addr);
    this->port = port;
    connect_socket = NULL;
    memset (&socket_addr, 0, sizeof (socket_addr));
}

int Socket::connect ()
{
    if ((connect_socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        return (int)SocketReturnCodes::CREATE_SOCKET_ERROR;
    }

    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons (port);
    if (inet_pton (AF_INET, ip_addr, &socket_addr.sin_addr) == 0)
    {
        return (int)SocketReturnCodes::PTON_ERROR;
    }

    // ensure that library will not hang in blocking recv/send call
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt (connect_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof (tv));
    setsockopt (connect_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof (tv));

    return (int)SocketReturnCodes::STATUS_OK;
}

int Socket::send (const char *data, int size)
{
    int res = sendto (connect_socket, (const char *)data, size, 0, (sockaddr *)&socket_addr,
        sizeof (socket_addr));
    return res;
}

int Socket::recv (void *data, int size)
{
    int len = sizeof (socket_addr);
    int res = recvfrom (connect_socket, (char *)data, size, 0, (sockaddr *)&socket_addr, &len);
    return res;
}

void Socket::close ()
{
    ::close (connect_socket);
    connect_socket = NULL;
}
#endif
