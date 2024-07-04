#include "headers.h"

void io_pipe_handling(char **pipesep, int num, struct allprocesses *procarr, int *numprocess, struct allprocesses *procarract, int *numprocessact,char* prevdir,char* homedir)
{
    int pipe1[num][2];
    // printf("%d\n",num);
    // for (int i = 0; i < num; i++){
    //     printf("%s\n",pipesep[i]);
    // }
    int storein=dup(STDIN_FILENO);
    int storeout=dup(STDOUT_FILENO);
    for (int i = 0; i < num; i++){
        // printf("inside");
        pipe(pipe1[i]);
        if (i != 0){
            // close(0);
            int val = dup2(pipe1[i - 1][0], STDIN_FILENO);
            // printf("%d\n",val);
            // close(pipe1[i-1][0]);
        }
        if (i != num - 1){
            // close(1);
            int val = dup2(pipe1[i][1], STDOUT_FILENO);
            // close(pipe1[i][1]);
        }
        // fdarrr
        // close fd1 fdd=fd0
        
        // if there is both pipe and io redirection then io redirection will be taken care of by this
        io_redirect(pipesep[i]);

        // executeprocess(pipesep[i],0,procarr,numprocess,procarract,numprocessact);
        // executeprocess(pipesep[i],0,procarr,&numprocess,procarract,&numprocessact);
        combinedexecute(pipesep[i],0,procarr,numprocess,procarract,numprocessact,prevdir,homedir);
        if(i!=0){
            close(pipe1[i-1][0]);
            dup2(storein,STDIN_FILENO);
        }
        if(i!=num-1){
            close(pipe1[i][1]);
            dup2(storeout,STDOUT_FILENO);
        }
    }
    close(pipe1[num][1]);
    dup2(storeout,STDOUT_FILENO);
    dup2(storein,STDIN_FILENO); 
}

int io_redirect(char* iosepcomm){
    char* commands[1024];
    char newstr[1024];
    strcpy(newstr,iosepcomm);
    char *token = strtok(newstr, "<>");
    int k=0;
    while (token!=NULL){
        commands[k]=token;
        k++;
        token=strtok(NULL,"<>");
    }
    char pwd[1024];
    getcwd(pwd,sizeof(pwd));
    // no < or > in the command
    if(k==1){
        return 0;
    }
    // this means only one < or > is there
    if(k==2){
        if(strstr(iosepcomm,"<")){
            char* token=strtok(commands[1]," \t");
            char pwd1[1026];
            sprintf(pwd1,"%s/%s",pwd,token);
            int val=access(pwd1,F_OK);
            if(val==-1){
                printf("No such file found\n");return 0;
            }
            val=access(pwd1,R_OK);
            if(val==-1){
                printf("File does not have read permissions\n");return 0;
            }
            val=open(pwd1,O_RDONLY);
            dup2(val,STDIN_FILENO);
            return val;
        }
        
        else if(strstr(iosepcomm,">>")){
            char* token=strtok(commands[1]," \t");
            char pwd1[1026];
            sprintf(pwd1,"%s/%s",pwd,token);
            int val=access(pwd1,F_OK);
            if(val==-1){
                val=open(pwd1,O_CREAT | O_APPEND | O_WRONLY ,0644);
            }else{
                val=open(pwd1,O_APPEND | O_WRONLY);
            }
            dup2(val,STDOUT_FILENO);
            return val;
        }
        
        else if(strstr(iosepcomm,">") ){
            // printf("in this");
            char* token=strtok(commands[1]," \t");
            char pwd1[1026];
            sprintf(pwd1,"%s/%s",pwd,token);
            int val=access(pwd1,F_OK);
            if(val==-1){
                // printf("HERE");
                val=open(pwd1, O_CREAT | O_TRUNC| O_WRONLY ,0644);
            }else{
                // printf("oho");
                val=open(pwd1,O_WRONLY| O_TRUNC );
                // printf("%d\n",val);
                // printf("here");
            }
            dup2(val,STDOUT_FILENO);
            return val;
        }
        
    }
    // this means < is definitely there along with 
    else if(k==3){
        
        if(strstr(iosepcomm,">>")){

            // first command is the input file so open it and redirect the input

            char* token=strtok(commands[1]," \t");
            char pwd1[1026];
            sprintf(pwd1,"%s/%s",pwd,token);
            int val=access(pwd1,F_OK);
            if(val==-1){
                printf("No such file found\n");return 0;
            }
            val=access(pwd1,R_OK);
            if(val==-1){
                printf("File does not have read permissions\n");return 0;
            }
            val=open(pwd1,O_RDONLY);
            dup2(val,STDIN_FILENO);
            
            // now the second command is the output file so open it and redirect the output
            char* token2=strtok(commands[2]," \t");
            char pwd2[1026];
            sprintf(pwd2,"%s/%s",pwd,token2);
            val=access(pwd2,F_OK);
            if(val==-1){
                val=open(pwd2,O_CREAT | O_APPEND | O_WRONLY ,0644);
            }else{
            val=open(pwd2,O_APPEND | O_WRONLY);
            }
                dup2(val,STDOUT_FILENO);
            return val;
        
        }else if(strstr(iosepcomm,">")){
            char* token=strtok(commands[1]," \t");
            char pwd1[1026];
            sprintf(pwd1,"%s/%s",pwd,token);
            int val=access(pwd1,F_OK);
            if(val==-1){
                printf("No such file found\n");return 0;
            }
            val=access(pwd1,R_OK);
            if(val==-1){
                printf("File does not have read permissions\n");return 0;
            }
            val=open(pwd1,O_RDONLY);
            dup2(val,STDIN_FILENO);
            
            char* token2=strtok(commands[2]," \t");
            char pwd2[1026];
            sprintf(pwd2,"%s/%s",pwd,token2);
            val=access(pwd2,F_OK);
            if(val==-1){
                val=open(pwd2,O_CREAT | O_WRONLY | O_TRUNC,0644);
            }else{
            val=open(pwd2,O_WRONLY | O_TRUNC);
            }
            dup2(val,STDOUT_FILENO);
            return val;
        }
        else{
            printf("Invalid command\n");
            return 0;
        }
    }
    // a > a,txt
    // a >> a.txt
    // a<a.txt > b.txt
    // a <a.txt >> b.txt
    // a < a.txt
    // printf("bahar b nikal gaya\n");
}