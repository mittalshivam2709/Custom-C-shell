#include "headers.h"

void fgbg(char* str,struct allprocesses* procarr,int* numprocess){
    // printf("yo");
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
    // int pid=atoi(commands[1]);
    // printf("%d\n",pid);    
    if(strstr(str,"bg")){
        int flag=0;
        for (int i = 0; i < *numprocess; i++){
            if(strcmp(procarr[i].procid,commands[1])==0){
                int pid=atoi(procarr[i].procid);
                flag=1;
                kill(pid,SIGCONT);
            }
        }
        if(flag==0){
            printf("No such process found\n");
        }
    }
    else if(strstr(str,"fg")){
        int flag=0;
        for (int i = 0; i < *numprocess; i++){
            if(strcmp(procarr[i].procid,commands[1])==0){
                int pid=atoi(commands[1]);
                flag=1;
                // kill(pid,SIGCONT);
                if(kill(pid,SIGCONT)==-1){
                    printf("No such Process found\n");
                    return;
                }
                int status;
                waitpid(pid,&status,0);
            }
        }
        if(flag==0){
            printf("No such process found\n");
        }
    }
}
