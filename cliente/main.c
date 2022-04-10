#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>




pthread_t *threadList;
int MODE=1;//the client mode 1 equals to MANUAL mode 2 equals to AUTO mode

void* sendToServer(void *arg){
    sleep(2);
    printf("%s\n", (char *)arg);
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
        threadList=(pthread_t *)malloc(threadsAmount * sizeof(pthread_t ));
        for (int i=0; i< sizeof(lista);i++){
            pthread_create(&threadList[i],NULL, sendToServer,lista[i]);

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
        return -1;
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




int main(){
    printf("SELECT THE MODE (TYPE THE NUMBER: \n\t1.MANUAL\n\t2.AUTOMATIC\n");
    scanf("%d",&MODE);
    if(MODE == 1){
        char fileName[]= "/home/koruss/Documents/Bretes/PlanificadorCPU/cliente/manual.txt";
        int count = countLines(fileName);
        char **list =  readFile(fileName, count);

        manual(list, count);
    }
    if(MODE == 2){
        //AUTO MODE GOES HERE
    }
    else{
        //retry
        printf("SELECT A VALID MODE");
    }
    pthread_exit(0);
    return 0;
}


