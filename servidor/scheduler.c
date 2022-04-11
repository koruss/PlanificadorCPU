#include "scheduler.h"
#include <unistd.h>

static int EXEC_PROC_AMOUNT = 0;
static int CPU_WAITING_SECS = 0;
static int PID = 0;
static int CPU_ACTIVE = 1;
static PCB *last_inserted = NULL;
static LIST_HEAD(pcb_list, _PCB) pcbs;
static LIST_HEAD(done_pcbs, _PCB) completed;

void add_pcb(PCB *pcb_input){
    // Insert the PCB to the JobScheduler queue.
    if(last_inserted==NULL)
        LIST_INSERT_HEAD(&pcbs, pcb_input, pointers);
    else
        LIST_INSERT_AFTER(last_inserted, pcb_input, pointers);
    // Set the process state to ready.
    pcb_input->state='r';
    // Set the last inserted pointer.
    last_inserted = pcb_input;
}

void print_context_switch(PCB *pcb){
    printf("Process %d, with %d burst, and %d priority is now executing.", pcb->pid, pcb->burst, pcb->prio);
}

void print_all_pcbs(){
    PCB *pcb;
    LIST_FOREACH(pcb, &pcbs, pointers){
        print_pcb(pcb);

    }
}

void print_ready_pcbs(){
    PCB *pcb;
    LIST_FOREACH(pcb, &pcbs, pointers){
        if(pcb->state == 'r')
            print_pcb(pcb);
    }
}

void *start_job_scheduler(){
    printf("Starting Job Scheduler\n");
    LIST_INIT(&pcbs);
    LIST_INIT(&completed);

    PCB *pcb1 = create_pcb(++PID, 8, 8);
    PCB *pcb2 = create_pcb(++PID, 2, 2);
    PCB *pcb3 = create_pcb(++PID, 3, 3);

    add_pcb(pcb1);
    add_pcb(pcb2);
    add_pcb(pcb3);

    return NULL;
}

void start_fifo(){
    PCB *head = NULL;
    while(CPU_ACTIVE){
        head = LIST_FIRST(&pcbs);
        if(head==NULL){
            printf("Queue empty, waiting for new processes.\n");
            sleep(1);
        }
        else{
            // Set the state of the PCB as running.
            head->state = 'R';
            print_context_switch(head);
            sleep(head->burst);
            LIST_REMOVE(head, pointers);
            LIST_INSERT_HEAD(&completed, head, pointers);
            head->state = 'T';
        }
    }

}

void* start_cpu_scheduler(void* void_arg){
    char* arg = void_arg;
    // Sleep the thread to let the process list to be populated.
    usleep(200);
    if (strcmp(arg, "fifo") == 0)
    {
        printf("Starting CPU scheduler with FIFO\n");
        start_fifo();
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

