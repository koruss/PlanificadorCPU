#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int i, argc;
    char **argv;
} connection_t;



void * processClient(void * ptr)
{
    connection_t * conn;
    conn = (connection_t *)ptr;

    int port;
    int sock = -1;
    struct sockaddr_in address;
    struct hostent * host;
    int len;

    /* checking commandline parameter */
    if (conn->argc != 4)
    {
        printf("usage: %s hostname port text\n", conn->argv[0]);
    }

    /* obtain port number */
    if (sscanf(conn->argv[2], "%d", &port) <= 0)
    {
        fprintf(stderr, "%s: error: wrong parameter: port\n", conn->argv[0]);
    }

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", conn->argv[0]);
    }

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    host = gethostbyname(conn->argv[1]);
    if (!host)
    {
        fprintf(stderr, "%s: error: unknown host %s\n", conn->argv[0], conn->argv[1]);

    }
    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
    {
        fprintf(stderr, "%s: error: cannot connect to host %s\n", conn->argv[0], conn->argv[1]);

    }

    /* send text to server */
    printf("%d\n", conn->i);
    len = strlen(conn->argv[3]);
    write(sock, &len, sizeof(int));
    write(sock, conn->argv[3], len);

    /* close socket */
    close(sock);


}

void r(int i){
    printf("%d\n", i);
}

int main(int argc, char **argv)
{
    connection_t * connection;
    pthread_t thread;


    connection = (connection_t *)malloc(sizeof(connection_t));
    int i=0;
    connection->i = i;
    connection->argv = argv;
    connection->argc = argc;
    while(1){
        pthread_create(&thread, 0, processClient, (void *)connection);
        pthread_detach(thread);

        sleep(5);
        ++i;
    }
    pthread_exit(0);

    return 0;
}
