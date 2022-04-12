#ifndef CONNECTION_H
#define CONNECTION_H

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

typedef struct
{
    int sock;
    struct sockaddr address;
    int addr_len;
} connection_t;



#endif // CONNECTION_H
