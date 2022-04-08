#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "jobsched.h"

void* startCpuScheduler(void* void_arg){
    char* arg = void_arg;
    if (strcmp(arg, "fifo") == 0)
    {
            printf("fifo inserted\n");
    }
    else if (strcmp(arg, "sjf") == 0)
    {
            printf("sjf inserted\n");
    }
    else if (strcmp(arg, "hpf") == 0)
    {
            printf("hpf inserted\n");
    }
    else if (strcmp(arg, "roundrobin") == 0)
    {
            printf("round robin inserted\n");
    }
    else
    {
        printf("invalid expression inserted\n");
    }
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t tid0;
    const char* arg1 = argv[1];

    // Create the thread for the CPU Scheduler.
    pthread_create(&tid0, NULL, startCpuScheduler, (void *) arg1);

    pthread_exit(NULL);
    return 0;
}
