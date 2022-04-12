#include "scheduler.h"

static int EXEC_PROC_AMOUNT = 0;
static int CPU_WAITING_SECS = 0;
static int PID = 0;
static PCB *last_inserted = NULL;
LIST_HEAD(pcb_list, _PCB) pcbs;

int sock = -1;

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
    printf("CPU SCHEDULER - Process %d, with %d burst, and %d priority is now executing.\n", pcb->pid, pcb->burst, pcb->prio);
    fflush(stdout);
}

void print_all_pcbs(){
    PCB *pcb;
    printf("****Printing all PCBs****\n");
    LIST_FOREACH(pcb, &pcbs, pointers){
        print_pcb(pcb);

    }
    fflush(stdout);
}

void print_ready_pcbs(){
    printf("****Printing ready PCBs****\n");
    PCB *pcb;
    LIST_FOREACH(pcb, &pcbs, pointers){
        if(pcb->state == 'r')
            print_pcb(pcb);
    }
    fflush(stdout);
}

void print_terminated_pcbs(){
    printf("****Printing terminated PCBs****\n");
    PCB *pcb;
    LIST_FOREACH(pcb, &pcbs, pointers){
        if(pcb->state == 't')
            print_pcb(pcb);
    }
    fflush(stdout);
}

void * process(void * ptr)
{
    char* hello = "Enviado desde el server";
    PCB *pcbcito = NULL;
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


        const char s[2] = ",";
        int burst = atoi(strtok(buffer, s));
        char *token =strtok(NULL,s);
        int priority = atoi(token);
        printf("JOB SCHEDULER - Process received: BURST: %d  PRIORITY: %d\n",burst,priority);
        send(conn->sock, hello, strlen(hello), 0);
        pcbcito =create_pcb(++PID, priority, burst);
        add_pcb(pcbcito);
        fflush(stdout);
        free(buffer);
    }


    /* close socket and clean up */
    close(conn->sock);
    free(conn);
    pthread_exit(0);
}

void *start_job_scheduler(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    printf("Starting Job Scheduler\n");
    LIST_INIT(&pcbs);

    struct sockaddr_in address;
    connection_t * connection;
    pthread_t thread;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "%s: error: cannot create socket\n", "Server");
        return NULL;
    }

    /* bind socket to port */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        fprintf(stderr, "%s: error: cannot bind socket to port %d\n", "Server", PORT);
        return NULL;
    }

    /* listen on port */
    if (listen(sock, 100) < 0)
    {
        fprintf(stderr, "%s: error: cannot listen on port\n", "Server");
        return NULL;
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



    return NULL;
}

PCB *get_next_fifo(){
    PCB *pcb=NULL;
    LIST_FOREACH(pcb, &pcbs, pointers){
        if(pcb->state == 'r')
            return pcb;
    }
    return NULL;
}

void start_fifo(){
    PCB *head = NULL;
    while(1){
        head = get_next_fifo();
        if(head==NULL){
            //printf("Queue empty, waiting for new processes.\n");
            sleep(1);
        }
        else{
            // Set the state of the PCB as running.
            head->state = 'R';
            print_context_switch(head);
            sleep(head->burst);
            // Set the state of the PCB as terminated.
            head->state = 't';
            printf("\nJOB SCHEDULER - Process with PID %d has terminated execution.\n", head->pid);
        }
    }
}

PCB *get_next_hpf(){
    PCB *pcb = NULL, *hp_pcb= NULL;
    LIST_FOREACH(pcb, &pcbs, pointers){
        if(pcb->state=='r'){
            if (hp_pcb == NULL) // First execution
                hp_pcb = pcb;
            else if(pcb->prio > hp_pcb->prio) // New elemnt has higher priority
                hp_pcb = pcb;
            else if (pcb->prio == hp_pcb->prio) // Tie-breaker in case of equal prio.
                if(hp_pcb->pid > pcb->pid)
                    hp_pcb = pcb;
        }
    }
    return hp_pcb;

}

void start_hpf(){
    PCB *head = NULL;
    while(1){
        head = get_next_hpf();
        if(head==NULL){
            //printf("Queue empty, waiting for new processes.\n");
            sleep(1);
        }
        else{
            // Set the state of the PCB as running.
            head->state = 'R';
            print_context_switch(head);
            sleep(head->burst);
            // Set the state of the PCB as terminated.
            head->state = 't';
            printf("\nJOB SCHEDULER - Process with PID %d has terminated execution.\n", head->pid);
        }
    }
}

void* start_cpu_scheduler(void* void_arg){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
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
        printf("Starting CPU scheduler with HPF\n");
        start_hpf();
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

