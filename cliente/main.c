#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/queue.h>

// Global
char *HOST_NAME = "localhost";
int PORT = 8089;

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
    int sock = -1;
    struct sockaddr_in address;
    struct hostent * host = gethostbyname(HOST_NAME);
    int len;
    char buffer[1024] = { 0 };

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (!host)
    {
        fprintf(stderr, "%s: error: unknown host %s\n", "Client", HOST_NAME);
    }

    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
    {
        fprintf(stderr, "%s: error: cannot connect to host %s\n", "Client", HOST_NAME);
    }

    /* send text to server */
    len = strlen(param);
    write(sock, &len, sizeof(int));
    write(sock, param, len);

    read(sock, buffer, 1024);
    printf("%s\n", buffer);
    /* close socket */
    close(sock);
    pthread_exit(0);
}

void* sendToServer(void *arg){
    sleep(2);
    printf("%s\n", (char *)arg);
    processClient(arg);
    return NULL;
}


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

        int burst = ( rand() % (max-min+1) )+ min;//calculates the burst based on min and max given
        char burstStr[sizeof(burst)];
        sprintf(burstStr , "%d" , burst);
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

void print_help(){
    printf("\nArgumentos de entrada:\n");
    printf("1 - Modo de operación \"auto\" 'o \"manual\".\n");
    printf("Si ingresa \"manual\", debe ingresar el siguiente argumento:\n");
    printf("\t2 - nombre del archivo utilizando una ruta relativa o absoluta.\n\n");
    printf("Si ingresa \"auto\" , debe ingresar los siguientes argumentos.");
    printf("\t2 - Valor entero mínimo para el BURST del proceso.\n");
    printf("\t3 - Valor entero máximo para el BURST del proceso.\n");
    printf("\t4 - Valor entero para la tasa de creación de procesos.\n");
}

int main(int argc, char **argv)
{
    if(argc == 3){
        char *arg1 = argv[1];
        char *arg2 = argv[2];

        if (strcmp(arg1, "manual") == 0)
        {
            printf("Starting client in manual execution mode.\n");
            int count = countLines(arg2);
            char **list =  readFile(arg2, count);
            manual(list, count);
        }else{
            print_help();
            return 1;
        }
    }else if(argc == 5){
        char *arg1 = argv[1]; //auto
        char *arg2 = argv[2]; //min burst
        char *arg3 = argv[3]; //max burst
        char *arg4 = argv[4]; //tasa creacion
        if (strcmp(arg1, "auto") == 0)
        {
            printf("Starting client in automatic execution mode.\n");
            autoMode(atoi(arg2), atoi(arg3), atoi(arg4));
        }else{
            print_help();
            return 1;
        }
    }else{
        printf("Cantidad incorrecta de argumentos ingresados.");
        print_help();
        return 1;
    }

    pthread_exit(0);

    return 0;
}


