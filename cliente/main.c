#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/queue.h>

typedef struct connection_t
{
    int i, argc;
    char **argv;
} connection_t;


typedef struct _Node {
    LIST_ENTRY(_Node) pointers;
    pthread_t thread;
}_Node;

_Node *createNode(pthread_t thread){
    _Node *nodito = (_Node *)malloc(sizeof(_Node));
    nodito->thread=thread;

}
pthread_t *threadListManual;
LIST_HEAD(Node_list, _Node) threadListAuto;
int MODE=1;//the client mode 1 equals to MANUAL mode 2 equals to AUTO mode



void * processClient(void * param)
{
    connection_t * conn;

    int port;
    int sock = -1;
    struct sockaddr_in address;
    char *host_name = "localhost";
    struct hostent * host = gethostbyname(host_name);
    int len;



    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("%s\n", "(char *)");

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(8980);
    printf("%s\n", "(char *)arg");
//    host = gethostbyname(host_name);
    if (!host)
    {
        fprintf(stderr, "%s: error: unknown host %s\n", conn->argv[0], conn->argv[1]);

    }


    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
    {
        fprintf(stderr, "%s: error: cannot connect to host %s\n", conn->argv[0], conn->argv[1]);

    }

    /* send text to server */
//    len = strlen("conn->argv[3]");
//    write(sock, &len, sizeof(int));
//    write(sock, "conn->argv[3]", len);
    len = strlen(param);
    write(sock, &len, sizeof(int));
    write(sock, param, len);

    /* close socket */
    close(sock);


}

void* sendToServer(void *arg){
    sleep(2);
    printf("%s\n", (char *)arg);
    processClient(arg);
    return NULL;
}


//void* manual(void * arg){
//    char fileName[]= "/home/koruss/Documents/Bretes/PlanificadorCPU/cliente/manual.txt";
//    FILE *fp = fopen(fileName, "r");
//    const char s[2] = ","; ///importante para aplicar en el server
//    char *token;
//    int i = 0;
//    struct process p1;
//    int ch = 6; //the amount of threads
//    if(fp != NULL)
//    {
//        threadList=(pthread_t *)malloc(ch * sizeof(pthread_t ));
//        char line[20];///importante para aplicar en el server
//        while(fgets(line, sizeof line, fp) != NULL)// read line by line
//        {
//            p1.burst = atoi(strtok(line, s));//read the burst part and save it in the struct
//            token = strtok(NULL,s);//read the second part of the line
//            p1.priority = atoi(token);//save it in the struct
//            p1.id = i;
//            //printf("Burst = %d\t, Priority = %d \n", p1.burst, p1.priority);
//            pthread_create(&threadList[i],NULL, sendToServer,(void *)&p1);
//            i = i+1;
//            //here we should call the socket emit fuction
//            int value = (rand()%6) + 3;
//            sleep( value ); //a number between 3 to 8
//        }
//        for(i=0;i<ch;i++)
//        {
//            pthread_join(threadList[i],NULL);
//        }
//        fclose(fp);//close the document
//    }
//    else {
//        printf("%s", "An Error Ocurred Reading the file" );
//    }
//    return NULL;
//}


void manual(char **lista, int threadsAmount){
    if( lista != NULL){
        threadListManual=(pthread_t *)malloc(threadsAmount * sizeof(pthread_t ));
        for (int i=0; i< sizeof(lista);i++){
            pthread_create(&threadListManual[i],NULL, sendToServer,lista[i]);

            int value = (rand()%6) + 3;
            sleep(value); //a number between 3 to 8
        }
    }
    else{
        printf("%s", "The list is empty");
    }
}

char **readFile(char *fileName,int length){
    char BUFFER[4096];
    char **list;
    FILE *fp = fopen(fileName, "r");// we open the file
    if(fp !=NULL){
        list = malloc(length * sizeof(char *));
        char line[20];
        int i =0;
        while(fgets(line, sizeof line, fp) != NULL)// read line by line
        {
            list[i] = malloc(length * sizeof(char));
            strcpy(list[i] , line);
            //printf("%s", list[i]);
            i= i+1;

        }
        fclose(fp);//close the document
        return list;

    }
    else{
        printf("%s", "An Error Ocurred Reading the file" );
        return NULL;
    }
}



int countLines(char *fileName){
    FILE *fp = fopen(fileName, "r");// we open the file
    int i = 0;
    if(fp != NULL)
    {
        char line[20];
        while(fgets(line, sizeof line, fp) != NULL)// read line by line
        {
            i = i+1;
        }
        return i;
    }
    else
    {
        printf("%s", "An Error Ocurred Reading the file" );
        return -1;
    }
    fclose(fp);//close the document
}



void autoMode(int min, int max,int ratio){
    while(1){
        int priority = (rand()%5) + 1;//calculates the priority between (1- 5)
        char priorityStr[sizeof(priority)];
        sprintf(priorityStr, "%d", priority);// converts the priority to a string

//        printf("%s", "priority: ");
//        printf("%s", priorityStr);

        int burst = ( rand() % (max-min+1) )+ min;//calculates the burst based on min and max given
        char burstStr[sizeof(burst)];
        sprintf(burstStr , "%d" , burst);
//        printf("%s\n","burst ");
//        printf("%s\n", burstStr);
        const char s[2] = ",";

        char line[20]="";
        strcat(line , burstStr);
        strcat(line,s);
        strcat(line,priorityStr);


        pthread_t thread;
        _Node *node = createNode(thread);
        pthread_create(&node->thread,NULL, sendToServer,line);

        LIST_INSERT_HEAD(&threadListAuto, node, pointers);

        sleep(ratio);
    }
}


int main(int argc, char **argv)
{
    connection_t * connection;
    pthread_t thread;


    connection = (connection_t *)malloc(sizeof(connection_t));
    int i=0;
    connection->i = i;
    connection->argv = argv;
    connection->argc = argc;

    printf("SELECT THE MODE (TYPE THE NUMBER): \n\t1.MANUAL\n\t2.AUTOMATIC\n");
    scanf("%d",&MODE);
    if(MODE == 1){
        char fileName[]= "/home/anner/U/SO/PlanificadorCPU/cliente/manual.txt";
        int count = countLines(fileName);
        char **list =  readFile(fileName, count);
        manual(list, count);
    }
    if(MODE == 2){
        int min =0;
        int max=0;
        int ratio=0;
        printf("\nIngrese el valor minimo del BURST \n");
        scanf("%d",&min);
        printf("\nIngrese el valor maximo del BURST \n");
        scanf("%d",&max);
        printf("\nIngrese la tasa de creacion de procesos \n");
        scanf("%d",&ratio);
        autoMode(min, max,ratio);
    }



    else{
        //retry
        printf("SELECT A VALID MODE");
    }
    pthread_exit(0);


    return 0;
}


