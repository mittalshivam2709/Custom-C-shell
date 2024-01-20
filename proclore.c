#include "headers.h"

int finddigits(int val){
    int store=val;
    int digits=0;
    while (store){
        store=store/10;
        digits++;
    }
    return digits;
}

char* converttostring(int num){
    int digits=finddigits(num);
    char* retstr=(char*)malloc(sizeof(char)*(digits+1));
    for (int i = digits-1; i >= 0; i--){
        int a=num%10;
        retstr[i]=a+'0';
        num=num/10;
    }
    retstr[digits]='\0';
    return retstr;
}

void proclore(char* str){
    int targetpid;
    int val=0;
    char* arr[1000];
    if(strlen(str)==8){
        int currid=getpid();
        // printf("%d\n",currid);
        arr[0]=converttostring(currid);
        // printf("\n%s\n",arr[0]);
        val=1;
    }
    else{
        char* token=strtok(str," ");
        token=strtok(NULL," ");
        while (token!=NULL){
            arr[val++]=token;
            token=strtok(NULL," ");
        }
    }
    for (int i = 0; i < val; i++){
        struct Processinfo info;
        // char* path=(char*)malloc(sizeof(char)*1024);
        char path[1024];
        strcpy(path,"/proc/");
        // printf("\n%s\n",arr[i]);
        strcat(path,arr[i]);
        
        strcat(path,"/status");
        // printf(" path is %s\n",path);

        // printf("\n%s\n",path);
        FILE* fp=fopen(path,"r");
        // if(fp==NULL){
        //     printf("Invalid input !\n");return;
        // }else{
        //     printf("else");
        // }
        // printf("yoyoyoyoy");
        char scanfile[1024*1024]; // this might create a problem if fgets ko seek_cur nahi kiya toh 
        int count=0;
        // char* temp;
        // int iter=0;
        while(count!=4){
            // printf("inside");
            // iter++;
            fgets(scanfile,sizeof(scanfile),fp);
            // printf(" %d ",count);
            if(sscanf(scanfile,"Pid: %d",&(info.pid))==1){
                count++;
            }
            if(sscanf(scanfile,"State: %c",&(info.status))==1){
                count++;
            }           
            if(sscanf(scanfile,"Tgid: %d",&(info.groupnum))==1){
                count++;
            } 
            if(sscanf(scanfile,"VmSize: %d",&(info.memory_occupied))==1){
                count++;
            }
        }
        // printf("%d ",iter);
        // printf(" came out of while loop ");


        int pgid = getpgrp();
        pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);

        if (pgid != fg_pgid) {
            printf("pid: %d\nprocess Status: %d\nprocess Group: %c\nVirtual memory: %d\n",info.pid,info.groupnum,info.status,info.memory_occupied);
        }
        else {
            printf("pid: %d\nprocess Status: %c+\nprocess Group: %d\nVirtual memory: %d\n",info.pid,info.status,info.groupnum,info.memory_occupied);
        }
        char buffer[1024];
        long len=readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        
        if (len!=-1) {
            buffer[len]='\0';
            printf("executable Path: %s\n", buffer);
        } 
        else {
            perror("readlink");
        }


    }    
}