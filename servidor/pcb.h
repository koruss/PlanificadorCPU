#ifndef PCB_H
#define PCB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <string.h>

// Process Control Block struct
typedef struct _PCB {
    LIST_ENTRY(_PCB) pointers;
    int pid, prio, burst;
    char state; //new, ready, running, waiting, terminated
}PCB;

PCB *create_pcb(int _pid, int _prio, int _burst);

void print_pcb(PCB *pcb);

#endif // PCB_H
