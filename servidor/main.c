
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <linux/in.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "scheduler.h"


static int KEY_D = 1;

    pthread_t tid0, tid1, tid2;

void print_help(){
    printf("\n****Help menu****\n");
    printf("\tPress r to show ready PCBs.\n");
    printf("\tPress t to show terminated PCBs.\n");
    printf("\tPress a to show all PCBs.\n");
    printf("\tPress q to quit the program.\n");
    printf("\tPress h to see this help menu.\n");
}

void quit_program(){
    KEY_D = 0;
    pthread_cancel(tid0);
    pthread_cancel(tid1);
    print_stats();
    print_tat_wt_table();
    printf("\nQuitting program...\n");
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

        if(user_input == 't')
            print_terminated_pcbs();
        if(user_input == 'a')
            print_all_pcbs();
    }
    pthread_exit(0);
    return 0;
}

int main(int argc, char ** argv)
{
    const char* arg1 = argv[1];

    //Create the thread for the Job Scheduler.
    pthread_create(&tid0, NULL, start_job_scheduler, NULL);

    // Create the thread for the CPU Scheduler.
    pthread_create(&tid1, NULL, start_cpu_scheduler, (void *) arg1);

    // Create the thread for the Keyboard daemon.
    pthread_create(&tid2, NULL, start_keyboard_daemon(), NULL);


    return 0;
}
