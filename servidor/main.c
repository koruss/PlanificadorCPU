
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "scheduler.h"


int PORT = 8089;
static int KEY_D = 1;

typedef struct
{
    int sock;
    struct sockaddr address;
    int addr_len;
} connection_t;

void * process(void * ptr)
{
    char * buffer;
    int len;
    connection_t * conn;
    long addr = 0;

    if (!ptr) pthread_exit(0);
    conn = (connection_t *)ptr;

    /* read length of message */
    read(conn->sock, &len, sizeof(int));
    if (len > 0)
    {
        addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
        buffer = (char *)malloc((len+1)*sizeof(char));
        buffer[len] = 0;

        /* read message */
        read(conn->sock, buffer, len);

        /* print message */
        printf("%d.%d.%d.%d: %s\n",
            (int)((addr      ) & 0xff),
            (int)((addr >>  8) & 0xff),
            (int)((addr >> 16) & 0xff),
            (int)((addr >> 24) & 0xff),
            buffer);
        free(buffer);
    }


    /* close socket and clean up */
    close(conn->sock);
    free(conn);
    pthread_exit(0);
}

int main(int argc, char ** argv)
{
    int sock = -1;
    struct sockaddr_in address;
    int port;
    connection_t * connection;
    pthread_t thread;

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", "Server");
        return -3;
    }

    /* bind socket to port */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        fprintf(stderr, "%s: error: cannot bind socket to port %d\n", "Server", PORT);
        return -4;
    }

    /* listen on port */
    if (listen(sock, 5) < 0)
    {
        fprintf(stderr, "%s: error: cannot listen on port\n", "Server");
        return -5;
    }

    printf("%s: ready and listening\n", "Server");

    while (1)
    {
        /* accept incoming connections */
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = accept(sock, &connection->address, &connection->addr_len);
        if (connection->sock <= 0)
        {
            free(connection);
        }
        else
        {
            /* start a new thread but do not wait for it */
            pthread_create(&thread, 0, process, (void *)connection);
            pthread_detach(thread);
        }
    }
}



void print_help(){
    printf("\n****Help menu****\n");
    printf("\tPress r to show ready PCBs.\n");
    printf("\tPress q to quit the program.\n");
    printf("\tPress h to see this help menu.\n");
}

void quit_program(){
    printf("Quitting keyboard daemon...");
    KEY_D = 0;
}

void *start_keyboard_daemon(){
    int user_input;
    usleep(400);
    printf("Starting keyboard daemon\n");
    while(KEY_D){
        printf( "Enter a command (h for help):");
        user_input = getchar();
        if(user_input == 'r')
            print_ready_pcbs();

        if(user_input == 'h')
            print_help();

        if(user_input == 'q')
            quit_program();
    }
    return 0;
}


int main(int argc, char** argv)
{
    pthread_t tid0, tid1, tid2;
    const char* arg1 = argv[1];

    //Create the thread for the Job Scheduler.
    pthread_create(&tid0, NULL, start_job_scheduler, NULL);

    // Create the thread for the CPU Scheduler.
    pthread_create(&tid1, NULL, start_cpu_scheduler, (void *) arg1);

    // Create the thread for the Keyboard daemon.
    pthread_create(&tid2, NULL, start_keyboard_daemon(), NULL);


    pthread_exit(NULL);

    return 0;
}
