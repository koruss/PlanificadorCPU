#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"

// Adds a PCB to the JobScheduler queue.
void add_pcb(PCB *pcb_input);

void print_all_pcbs();

void print_ready_pcbs();

void *start_job_scheduler();

void* start_cpu_scheduler(void* void_arg);

#endif // SCHEDULER_H
