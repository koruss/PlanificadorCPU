#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct process{
    int id,priority,burst;
};


pthread_t *threadList;

void* sendToServer(void * arg){
    struct process *args = arg;
    printf("Id = %d\t, Burst = %d\t, Priority = %d \n",args->id, args->burst, args->priority);

}

void* manual(void * arg){
    char fileName[]= "/home/koruss/Documents/Bretes/PlanificadorCPU/cliente/manual.txt";
    FILE *fp = fopen(fileName, "r");
    const char s[2] = ",";
    char *token;
    int i = 0;
    struct process p1;
    int ch = 6; //the amount of threads
    if(fp != NULL)
    {
        threadList=(pthread_t *)malloc(ch * sizeof(pthread_t ));
        char line[20];
        while(fgets(line, sizeof line, fp) != NULL)// read line by line
        {

            p1.burst = atoi(strtok(line, s));//read the burst part and save it in the struct
            token = strtok(NULL,s);//read the second part of the line
            p1.priority = atoi(token);//save it in the struct
            p1.id = i;
            //printf("Burst = %d\t, Priority = %d \n", p1.burst, p1.priority);



            pthread_create(&threadList[i],NULL, sendToServer,(void *)&p1);
            i = i+1;

            //here we should call the socket emit fuction

            int value = (rand()%6) + 3;
            sleep( value ); //a number between 3 to 8




        }
        for(i=0;i<ch;i++)
            {
                pthread_join(threadList[i],NULL);
            }
        fclose(fp);//close the document
    }
    else {
        printf("%s", "An Error Ocurred Reading the file" );
    }
    return NULL;
}

//char **read(route){


//}


int main()
{
    pthread_t mainThread;
    pthread_create(&mainThread,NULL, manual,NULL);

    pthread_exit(0);

    return 0;
}


