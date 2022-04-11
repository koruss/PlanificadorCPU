#include "pcb.h"


PCB *create_pcb(int _pid, int _prio, int _burst){
    PCB *pcb = (PCB *)malloc(sizeof(PCB));
    // Se crea el proceso con estado 'n' de 'new'.
    pcb->state = 'n';
    pcb->burst = _burst;
    pcb->pid = _pid;
    pcb->prio = _prio;
    return pcb;
}

void print_pcb(PCB *pcb){
    printf("PCB pid: %d\n", pcb->pid);
    printf("\t Burst: %d\n", pcb->burst);
    printf("\t Priority: %d\n", pcb->prio);
    switch(pcb->state) {

    case 'n':
        printf("\t state: New\n");
        break;
    case 'r':
        printf("\t state: Ready\n");
        break;
    case 'R':
        printf("\t state: Running\n");
        break;
    case 'w':
        printf("\t state: Waiting/Blocked\n");
        break;
    case 't':
        printf("\t state: Terminated\n");
        break;
    default : /* Optional */
        printf("\t state: INVALID STATE\n");
    }
}
