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
        arr[0]=converttostring(currid);
        val=1;
    }
    else{
        char* token=strtok(str," \t");
        token=strtok(NULL," \t");
        while (token!=NULL){
            arr[val++]=token;
            token=strtok(NULL," \t");
        }
    }
    for (int i = 0; i < val; i++){
        struct Processinfo info;

        char path[1024];

        strcpy(path,"/proc/");
        strcat(path,arr[i]);        
        strcat(path,"/status");

        FILE* fp=fopen(path,"r");
        if(fp==NULL){
            printf("Invalid input !\n");
            printf("Error opening file /proc/%s/status file \n",arr[i]);
            return;
        }

        char scanfile[1024*1024];
        int count=0;

        // read the file 

        int c=0;bool f=0;
        while(count!=4){
            c++;
            fgets(scanfile,sizeof(scanfile),fp);
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
            if(c>1000000){
                f=true;
                break;
            }
        }
        if(f){
            printf("Coudln't read the file /proc/%s/status properly \n",arr[i]);
            continue;
        }


        int pgid = getpgrp();
        pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);

        // print + for foreground process 
        if (pgid != fg_pgid) {
            printf("pid: %d\nprocess Status: %d\nprocess Group: %c\nVirtual memory: %d\n",info.pid,info.status,info.groupnum,info.memory_occupied);
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
