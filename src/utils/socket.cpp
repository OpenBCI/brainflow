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
}

int Socket::connect ()
{
    WSADATA wsadata;
    int res = WSAStartup (MAKEWORD (2, 2), &wsadata);
    if (res != 0)
    {
        return (int)SocketReturnCodes::WSA_STARTUP_ERROR;
    }
    struct sockaddr_in socket_addr;
    memset (&socket_addr, 0, sizeof (socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons (port);
    socket_addr.sin_addr.s_addr = inet_addr (ip_addr);

    connect_socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (connect_socket == INVALID_SOCKET)
    {
        return (int)SocketReturnCodes::CREATE_SOCKET_ERROR;
    }

    // ensure that library will not hang in blocking recv/send call
    DWORD timeout = 3000;
    setsockopt (connect_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof (timeout));
    setsockopt (connect_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof (timeout));

    res = ::connect (connect_socket, (sockaddr *)&socket_addr, sizeof (socket_addr));
    if (res == SOCKET_ERROR)
    {
        return (int)SocketReturnCodes::CONNECT_ERROR;
    }

    return (int)SocketReturnCodes::STATUS_OK;
}

int Socket::send (const void *data, int size)
{
    int res = ::send (connect_socket, (char *)data, size, 0);
    if (res == SOCKET_ERROR)
    {
        return -1;
    }
    return res;
}

int Socket::recv (void *data, int size)
{
    int res = ::recv (connect_socket, (char *)data, size, 0);
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
    if (inet_aton (ip_addr, &socket_addr.sin_addr) == 0)
    {
        return (int)SocketReturnCodes::ATON_ERROR;
    }

    // ensure that library will not hang in blocking recv/send call
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt (connect_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof (tv));
    setsockopt (connect_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof (tv));

    return (int)SocketReturnCodes::STATUS_OK;
}

int Socket::send (const void *data, int size)
{
    int res = sendto (connect_socket, (const char *)data, size, 0,
        (const struct sockaddr *)&socket_addr, sizeof (socket_addr));
    return res;
}

int Socket::recv (void *data, int size)
{
    unsigned int len = (unsigned int)sizeof (socket_addr);
    int res =
        recvfrom (connect_socket, (char *)data, size, 0, (struct sockaddr *)&socket_addr, &len);
    return res;
}

void Socket::close ()
{
    ::close (connect_socket);
    connect_socket = NULL;
}
#endif
