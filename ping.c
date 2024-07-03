#include "headers.h"

void ping(char* str,struct allprocesses* procarr,int* num){
    char* commands[1024];
    char newstr[1024];
    strcpy(newstr,str);
    char *token = strtok(newstr, " \t");
    int k=0;
    while (token!=NULL){
        commands[k]=token;
        k++;
        token=strtok(NULL," \t");
    }
    int pid=atoi(commands[1]);      
    int signal=atoi(commands[2]);      
    printf("%d\n",signal);
    signal=signal%32;
    kill(pid,signal);
    printf("Sent signal %d to process with pid %d\n",signal,pid);
    
    // printf("hello");
}   