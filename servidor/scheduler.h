#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <unistd.h>
#include <pthread.h>
#include "pcb.h"
#include "connection.h"
#include <semaphore.h>

static int EXEC_PROC_AMOUNT;
static int CPU_WAITING_SECS;
static int PID;
static int CPU_ACTIVE;
static PCB *last_inserted;
static int PORT = 8089;
static sem_t SEM;


// Adds a PCB to the JobScheduler queue.
void add_pcb(PCB *pcb_input);

void print_all_pcbs();

void print_ready_pcbs();

void print_terminated_pcbs();

void *start_job_scheduler();

void* start_cpu_scheduler(void* void_arg);

#endif // SCHEDULER_H
