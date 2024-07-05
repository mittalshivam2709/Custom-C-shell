#include"headers.h"

void prompt(int isfirst,char* homedir){
        char *username = getenv("LOGNAME"); //LOGNAME first
        if (username == NULL) {
            username = getenv("USER"); // If LOGNAME is not available, try USER
        }
        char sysname[1024]; // check for inserting ~ when in home directory 
        gethostname(sysname,sizeof(sysname));
        printf("<");
        // printf(ANSI_COLOR_BLUE"%s@%s:%s>",username,sysname,pwd);
        printf(ANSI_COLOR_YELLOW"%s",username);
        printf("@");
        printf(ANSI_COLOR_BLUE"%s"ANSI_COLOR_RESET,sysname);
        printf(":");
        if(isfirst==1){
            char pwd[1024];
            getcwd(pwd,sizeof(pwd));
            printf("~>");
            return;
        }
        else{
            char pwd[1024];
            getcwd(pwd,sizeof(pwd));
            char* temp=(char*)malloc(sizeof(char)*1024);
            for (int i = 0; i < strlen(pwd); i++){
                temp[i]=pwd[i];
            }
            temp[strlen(pwd)]='\0';
            // printf("inside");
        //     // printf("\n homedir  is %s\n",homedir);
        //     // printf("\n pwd  is %s\n",pwd);
            if(strstr(pwd,homedir)){
                temp+=strlen(homedir);
            }
            if(strlen(temp)==0){
                printf("~");
            }
            printf("%s>",temp);    
        }
// printf("\nlo aa agye prompt me \n");
}

