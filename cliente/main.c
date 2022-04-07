#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct process{
    int id,priority,burst;
};

void manual(char *fileRoute){
    FILE *fp = fopen(fileRoute, "r");
    const char s[2] = ",";
    char *token;
    int i;
    struct process p1;
    if(fp != NULL)
    {
        char line[20];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            p1.burst = atoi(strtok(line, s));
            token = strtok(NULL,s);
            p1.priority = atoi(token);

            printf("x = %d\t, y = %d \n", p1.priority, p1.burst);
        }
        fclose(fp);

    }


    else {
        printf("%s", "An Error Ocurred Reading the file" );
    }
}


int main()
{
    char fileName[]= "/home/koruss/Documents/Bretes/PlanificadorCPU/cliente/manual.txt";
    manual(fileName);

    return 0;

}


