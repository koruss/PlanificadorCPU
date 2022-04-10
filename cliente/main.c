#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int sock;
    int len;
} connection_t;



void * processClient(void * ptr)
{
    connection_t * conn;
    conn = (connection_t *)ptr;
    int len;


    len = strlen("'argv[3]'");
    write(conn->sock, &len, sizeof(int));
    write(conn->sock, "'argv[3]'", len);
}

void r(int i){
    printf("%d\n", i);
}

int main(int argc, char ** argv)
{
    int port;
    int sock = -1;
    struct sockaddr_in address;
    struct hostent * host;
    connection_t * connection;
    int len;


    /* checking commandline parameter */
    if (argc != 4)
    {
        printf("usage: %s hostname port text\n", argv[0]);
        return -1;
    }

    /* obtain port number */
    if (sscanf(argv[2], "%d", &port) <= 0)
    {
        fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
        return -2;
    }

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
        return -3;
    }

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(8980);
    host = gethostbyname(argv[1]);
    if (!host)
    {
        fprintf(stderr, "%s: error: unknown host %s\n", argv[0], argv[1]);
        return -4;
    }
    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
    {
        fprintf(stderr, "%s: error: cannot connect to host %s\n", argv[0], argv[1]);
        return -5;
    }
    int i = 1;

    len = strlen("'argv[3]'");

    while(1){
        pthread_t thread;
        r(i);
        /* send text to server */
//        write(sock, &len, sizeof(int));
//        write(sock, "'argv[3]'", len);
        /* accept incoming connections */
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = sock;
        connection->len = len;

        /* start a new thread but do not wait for it */
        pthread_create(&thread, 0, processClient, (void *)connection);
        pthread_detach(thread);

        ++i;
        sleep(1);
    }

    /* close socket */
    close(sock);

    return 0;
}
