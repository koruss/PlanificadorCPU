#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int i;
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

//    /* checking commandline parameter */
//    if (argc != 4)
//    {
//        printf("usage: %s hostname port text\n", argv[0]);
//    }

//    /* obtain port number */
//    if (sscanf(argv[2], "%d", &port) <= 0)
//    {
//        fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
//    }

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (sock <= 0)
//    {
//        fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
//    }

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(8980);
    host = gethostbyname("localhost");

//    if (!host)
//    {
//        fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
//    }
    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
//    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
//    {
//        fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
//    }

    /* send text to server */
    len = strlen("argv[3]");
    printf("%d\n", conn->i);
    write(sock, &len, sizeof(int));
    write(sock, "argv[3]", len);

    /* close socket */
    close(sock);

    return 0;
}

void r(int i){
    printf("%d\n", i);
}

int main()
{
    connection_t * connection;
    pthread_t thread;


    connection = (connection_t *)malloc(sizeof(connection_t));
//    connection->argv = argv;
    int i=0;
    connection->i = i;
    while(1){
        pthread_create(&thread, 0, processClient, (void *)connection);
        pthread_detach(thread);

        sleep(1);
        ++i;
    }
    pthread_exit(0);

    return 0;
}
