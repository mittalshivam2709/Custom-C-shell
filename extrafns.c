#include "headers.h"


int numofcommands(char* str){
    char temp[strlen(str)+1];
    for (int i = 0; i < strlen(str); i++)
    {
        temp[i]=str[i];
    }
    
    char* token=strtok(temp,"&");
    int num=0;
    while (token!=NULL)
    {
        num++;
        // printf("in token %sdone",token);
        token=strtok(NULL,"&");
    }
    return num;

}


void executeprocess(char* str,int bg,int* arr,int* idx){
    int pid=fork();
    if(pid==0){
        // int status;
        // waitpid(-1,&status,0);
        // if(WIFEXITED(status)==1){
            // printf("command executed successfully");
        // }
        // else{
            // printf("command failed");
        // }
        // if(bg!=0){
            printf(" pid of child is %d",getpid());            //getpid();
            char* cmd[4];
            cmd[0]="sh";
            cmd[1]="-c";
            cmd[2]=str;
            cmd[3]=NULL;
            execvp("sh",cmd);
            perror("execvp");
            exit(EXIT_FAILURE);
        // }
        // else{
        //     exit(1);
        // }
    }
    else if(pid>0){
        // int status;
        printf("%d\n",pid);            //getpid();

        if(bg==0){
            int status;
            waitpid(pid,&status,0);
        }
        else{
            arr[(*idx)++]=pid;
        }
        // waitpid(pid, &status, 0);
        // if(WIFEXITED(status)==1){
        //     printf("executed normally");
        // }
        // else{
        //     printf("command failed");
        // }
    }
}

// a & b& c& d
// fork ()
// 2 proc
// child execute
// parent store pid

char** seperateands(char* str){
    // printf(" here %s ",str);
    int val=numofcommands(str);
    char** retcommands=(char**)malloc(sizeof(char*)*(val+1));
    char* token=strtok(str,"&");
    int i=0;
    while (token!=NULL)
    {
        retcommands[i++]=token;
        token=strtok(NULL,"&");
    }
    return retcommands;
}

void removespacesandtabs(char** comds,int num){
    for (int cn = 0; cn < num; cn++) // cn is comm no.
    {
        int val=strlen(comds[cn]);
        int i=0;
        while ((i<val)&&(comds[cn][i] == ' ' || comds[cn][i] == '\t')) {
            i++;
        }
        int j=strlen(comds[cn])-1;
        while ((j>=0)&&(comds[cn][j]==' '|| comds[cn][j]=='\t')){
            j--;
        }

        if(i<=j){
            char* finalstr=(char*)malloc(sizeof(char)*(j-i+2));
            for (int k = i; k <= j; k++){
                finalstr[k-i]=comds[cn][k];
            }
            finalstr[j-i+1]='\0';
            comds[cn]=finalstr;
            // Execute the command
            // execute_command(finalstr);
        }   
    }
    
}


