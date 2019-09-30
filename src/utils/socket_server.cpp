#include "socket_server.h"


///////////////////////////////
/////////// WINDOWS ///////////
//////////////////////////////
#ifdef _WIN32

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

SocketServer::SocketServer (
    const char *local_ip, const char *client_ip, int local_port, int client_port)
{
    strcpy (this->client_ip, client_ip);
    strcpy (this->local_ip, local_ip);
    this->client_port = client_port;
    this->local_port = local_port;
    server_socket = INVALID_SOCKET;
}

int SocketServer::bind ()
{
    WSADATA wsadata;
    int res = WSAStartup (MAKEWORD (2, 2), &wsadata);
    if (res != 0)
    {
        return (int)SocketReturnCodes::WSA_STARTUP_ERROR;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (local_port);
    if (inet_pton (AF_INET, local_ip, &server_addr.sin_addr) == 0)
    {
        return (int)SocketReturnCodes::PTON_ERROR;
    }
    server_socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == INVALID_SOCKET)
    {
        return (int)SocketReturnCodes::CREATE_SOCKET_ERROR;
    }

    if (::bind (server_socket, (const struct sockaddr *)&server_addr, sizeof (server_addr)) != 0)
    {
        return (int)SocketReturnCodes::CONNECT_ERROR;
    }

    // ensure that library will not hang in blocking recv/send call
    DWORD timeout = 3000;
    setsockopt (server_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof (timeout));
    setsockopt (server_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof (timeout));

    return (int)SocketReturnCodes::STATUS_OK;
}

int SocketServer::recv (void *data, int size)
{
    int len = sizeof (client_addr);
    int res = recvfrom (server_socket, (char *)data, size, 0, (sockaddr *)&client_addr, &len);
    if (res == SOCKET_ERROR)
    {
        return -1;
    }
    return res;
}

void SocketServer::close ()
{
    closesocket (server_socket);
    server_socket = INVALID_SOCKET;
    WSACleanup ();
}

///////////////////////////////
//////////// UNIX /////////////
///////////////////////////////
#else

#include <netinet/in.h>
#include <netinet/tcp.h>

SocketServer::SocketServer (const char *ip_addr, int port, int socket_type)
{
    strcpy (this->client_ip, client_ip);
    strcpy (this->local_ip, local_ip);
    this->client_port = client_port;
    this->local_port = local_port;
    server_socket = -1;
}

int SocketServer::bind ()
{
    server_socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket < 0)
    {
        return (int)SocketReturnCodes::CREATE_SOCKET_ERROR;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (local_port);
    if (inet_pton (AF_INET, local_ip, &server_addr.sin_addr) == 0)
    {
        return (int)SocketReturnCodes::PTON_ERROR;
    }

    if (bind (server_socket, (const struct sockaddr *)&server_addr, sizeof (server_addr)) != 0)
    {
        return (int)SocketReturnCodes::CONNECT_ERROR;
    }

    // ensure that library will not hang in blocking recv/send call
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt (server_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof (tv));
    setsockopt (server_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof (tv));

    return (int)SocketReturnCodes::STATUS_OK;
}

int SocketServer::recv (void *data, int size)
{
    unsigned int len = (unsigned int)sizeof (socket_addr);
    int res = recvfrom (server_socket, (char *)data, size, 0, (sockaddr *)&client_addr, &len);
    return res;
}

void SocketServer::close ()
{
    ::close (server_socket);
    server_socket = -1;
}
#endif
