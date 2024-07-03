#include "headers.h"

int comp(const void* a,const void*b){
    return strcmp (((struct allprocesses*)a)->procid,((struct allprocesses*)b)->procid);
}

void activities(char* str,struct allprocesses* procarr,int* numprocess){
    qsort(procarr,*numprocess,sizeof(struct allprocesses),comp);
    for (int i = 0; i < *numprocess; i++){
        char path[1024];
        strcpy(path,"/proc/");
        // printf("\n%s\n",arr[i]);
        strcat(path,procarr[i].procid);
        strcat(path,"/status");
        FILE* fp=fopen(path,"r");
        if(fp!=NULL){
            char ch;
            int count=0;
            char buffer[1024];
            while(count!=1){
                fgets(buffer,sizeof(buffer),fp);
                if(sscanf(buffer,"State: %c",&ch)==1){
                    count++;
                }
            }   
            if(ch=='T'){
                printf("%s : %s - Stopped \n",procarr[i].procid,procarr[i].procname);
            }
            else if(ch=='S'){
                printf("%s : %s - Running \n",procarr[i].procid,procarr[i].procname);
            }
        }
    }
}