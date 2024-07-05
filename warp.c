#include "headers.h"

void warp(char* str,char* prevdir,char* homedir){

    char* newstr=(char*)malloc(sizeof(char)*1024);
    strcpy(newstr,str);

    // bypass warp
    newstr+=3;

    // to copy in prevdir after completing
    char temp[1024];
    getcwd(temp,sizeof(temp));

    // only warp given as input
    if(newstr[1]=='\0'){
        chdir(homedir);
        char pwd[1024];
        getcwd(pwd,sizeof(pwd));
        printf("%s\n",pwd);
    }

    else{
        newstr+=1; 
        char* token=strtok(newstr," \t/");
        while(token!=NULL){
            
            if(token[0]=='-'){
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
                // multiple / seperated arguments given
                
                int val=chdir(token);
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
        }
    }
    strcpy(prevdir,temp);
}

