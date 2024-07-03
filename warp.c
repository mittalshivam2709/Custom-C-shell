#include "headers.h"

void warp(char* str,char* prevdir,char* homedir){
    // printf("here ");
    // error message when chdir fails
    // str+=3;
    char* newstr=(char*)malloc(sizeof(char)*1024);
    strcpy(newstr,str);
    newstr+=3;
    if(newstr[1]=='\0'){
        chdir(homedir);
        char pwd[1024];
        getcwd(pwd,sizeof(pwd));
        printf("%s\n",pwd);
    }
    else{
        newstr+=1;
        char* token=strtok(newstr," \t/");
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
                    printf("Invalid Input !!\n");return;
                }
                if(val==0){
                char pwd[1024];
                getcwd(pwd,sizeof(pwd));
                printf("%s\n",pwd);
                }
            }
            token=strtok(NULL," \t/");
            strcpy(prevdir,temp);
        }
    }
}

