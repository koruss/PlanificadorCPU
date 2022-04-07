#include <stdio.h>
#include "pcb.h"

void startJobScheduler(char* arg){
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
    else /* default: */
    {
        printf("invalid expression inserted\n");
    }
}

int main(int argc, char** argv)
{

    startJobScheduler(argv[1]);

    return 0;
}
