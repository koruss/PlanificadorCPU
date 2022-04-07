#ifndef PCB_H
#define PCB_H

#include <string.h>

// Process Control Block
struct PCB {
    int pid, prio, burst;
    char state; //new, ready, running, waiting, terminated
};

#endif // PCB_H
