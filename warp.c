#include "headers.h"

void warp(char* str,char* prevdir,char* homedir){
    // printf("here ");
    //*************** ../fold then warp - vala case dekhna baaki hai !!!!!!!!
    // error message when chdir fails

    str+=3;
    if(str[1]=='\0'){
        chdir(homedir);
        char pwd[1024];
        getcwd(pwd,sizeof(pwd));
        printf("%s\n",pwd);
    }
    else{
        str+=1;
        char* token=strtok(str," /");
        char temp[1024];
        while(token!=NULL){
            // printf("beg is %s ",prevdir);
            getcwd(temp,sizeof(temp));
            // printf("temp is %s ",temp);
            if(token[0]=='-'){
                // printf("here");
                // printf("%s ",prevdir);
                chdir(prevdir);
                char pwd[1024];
                getcwd(pwd,sizeof(pwd));
                printf("%s\n",pwd);
            }
            else if(token[0]=='~'){
                chdir(homedir);
                char pwd[1024];
                getcwd(pwd,sizeof(pwd));
                printf("%s\n",pwd);
            }
            else{
                // printf("hii");
                // printf(" %s ",token);
                
                int val=chdir(token);
                // printf("%d\n",val);
                if(val!=0){
                    printf("Invalid Input !!\n");
                }
                if(val==0){
                char pwd[1024];
                getcwd(pwd,sizeof(pwd));
                printf("%s\n",pwd);
                }
            }
            token=strtok(NULL," /");
            strcpy(prevdir,temp);
        }
    }
}

