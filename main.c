#include "headers.h"

// execute all the process in the child process and if foreground process is being executed then use a wait in parent for the same chidl
// print the output any time it does not matter however print the exit status just after the next prompt
// therefore store the pid in parent


int fgpid;
char prompthomedir[1024];

void sigint_handler(int sig) {
    if (fgpid != 0) {
        kill(fgpid, SIGINT);
    }
}

void sigtstp_handler(int sig) {
    if (fgpid != 0) {
        kill(fgpid, SIGTSTP);
        fgpid = 0;
        // setpgid(fgpid, fgpid);
        // tcsetpgrp(STDIN_FILENO, getpgrp()); // Set the terminal foreground process group
        // prompt(0, prompthomedir);
        // printf("\n");
    }
}
int ctrlzflag=0;

char path[1024];
int main() {                        
    char prevdir[1024];
    char homedir[1024];
    getcwd(prompthomedir,1024);
    int i=0;
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    struct allprocesses procarr[1024];
    struct allprocesses procarract[1024];
    int numprocess=0;
    int numprocessact=0;
    while (1) {
        // call prompt as required
        ctrlzflag=0;
        if(i==0){
            prompt(1,prompthomedir);
        }
        else{
            prompt(0,prompthomedir);
        }
        i++; //necessary for warp cwd
        if(i==1){
            getcwd(prevdir,sizeof(prevdir));
            getcwd(homedir,sizeof(homedir));
        }
        strcpy(path,homedir);
        strcat(path,"/history.txt");
        char input[1024];
        char* check=fgets(input, sizeof(input), stdin);
        if(check==NULL){
            kill(0,SIGTERM);
            printf("\n");
            break;
        }

        if(strcmp(check,"26")==0){
            if(ctrlzflag==1){
                continue;
            }
        }
        
        for (int check = 0; check < numprocess; check++){
            int pid=atoi(procarr[check].procid);
            int status;
            int procstat=waitpid(pid,&status,WNOHANG);
            if(procstat!=0 && procstat!=-1){
                // ended processes
                if(procarr[check].flag){
                    // print the info hence continue
                    continue;
                }
                else{
                    // info yet to be printed 
                    if(WIFEXITED(status)){
                        printf("%s exited normally (%d)\n",procarr[check].procname,pid);
                    }else if(WIFSIGNALED(status)){
                        printf("%s exited abnrmally (%d)\n",procarr[check].procname,pid);
                    }
                    // change the flag
                    procarr[check].flag=1;
                }
            }
        }
        
        char pastevents[11]="pastevents";
        FILE *fp = fopen(path, "r");
        if (fp==NULL){
            perror("Error opening file");
            return 0;
        }
        fseek(fp, 0, SEEK_END); // pointer set to next to eof
        long long int file_size = ftell(fp);
        long long int pos = file_size;
        int linenum=0;
        // printf("here is %c\n",fgetc(fp));
        char line[1024];
        while (linenum < 2 && pos >= 0){
            // printf("padharo");
            fseek(fp, --pos, SEEK_SET);
            if (fgetc(fp) == '\n'){
                linenum++;
            }
        }
      
        fgets(line,sizeof(line),fp);
        fclose(fp);
        
        if(strstr(input,pastevents)==NULL && strcmp(input,line)!=0){

            FILE *file = fopen(path, "a");
            if (file==NULL) {
                perror("Error opening the history file");
            }
            fprintf(file, "%s", input);
            fclose(file);
        }
       
        input[strlen(input) - 1] = '\0'; // \n replaced by \0
        if (strcmp(input, "exit") == 0) {
            // printf("heelo");
            break;
        }

        char* commands[1024];
        char *token = strtok(input, ";");
        int k=0;
 
        while (token!=NULL){
            commands[k]=token;
            k++;
            token=strtok(NULL,";");
        }
        
        //seperated by ; and stored in commmands which is array of pointers
        //loop for commands free of semicolons
        for (int j = 0; j < k; j++)
        {
            int onlybg=0;
            // check if u have only &
            int a;
            for (a = strlen(commands[j])-1; a >=0 ; a--){
                if(commands[j][a]==' '|| commands[j][a]=='\t'){}
                else{
                    if(commands[j][a]=='&'){ // & at the last
                        onlybg=1;
                    }
                    commands[j][a+1]='\0';
                    break;
                }
            }
            int pipe_or_io=0;
            int f=0;
            for (int p = 0; p < a+1; p++){
                if(commands[j][p]=='|'){
                    pipe_or_io=1;
                }if(commands[j][p]=='<'|| commands[j][p]=='>'){
                    f=1;
                }
            }            
            if(pipe_or_io && f){
                char* token=strtok(commands[j],"|");
                char** pipesep=(char**)malloc(sizeof(char*)*1024);
                int pipes=0;
                while(token!=NULL){
                    pipesep[pipes++]=token;
                    token=strtok(NULL,"|");
                }
                io_pipe_handling(pipesep,pipes,procarr,&numprocess,procarract,&numprocessact,prevdir,homedir);      
             
            }
            else if(f){
                int storein=dup(STDIN_FILENO);
                int storeout=dup(STDOUT_FILENO);
    
                int val=io_redirect(commands[j]);
                char* cmd[4];
                char* tkn=strtok(commands[j],"<>");
                combinedexecute(tkn,0,procarr,&numprocess,procarract,&numprocessact,prevdir,homedir);
                close(val);
                dup2(storein,STDIN_FILENO);
                dup2(storeout,STDOUT_FILENO);
    
            }else if(pipe_or_io){

                char* token=strtok(commands[j],"|");
                char** pipesep=(char**)malloc(sizeof(char*)*1024);
                int pipes=0;
                while(token!=NULL){
                    pipesep[pipes++]=token;
                    // printf("%s\n",token);
                    token=strtok(NULL,"|");
                }
                
                io_pipe_handling(pipesep,pipes,procarr,&numprocess,procarract,&numprocessact,prevdir,homedir);      
                
            }
            // printf("jere");
            else{
            // multiple command having spaces and tabs and & in between 
            int val=numofcommands(commands[j]);

            // formed array of strings which does not have & in between and seperated by spaces and tabs
            char** comds=seperateands(commands[j]); 

            // cmds has val num of commands
            // now remove spaces and tabs from each of the commands
            removespacesandtabs(comds,val);

            // now in comds[i] which has size=val we have the actual command that needs to be executed
            // the spaces in between will be taken care by strtok of individual cmd

            //************ now in comds
            
            // function call for the background process execution

            int forfor=val;
            if(onlybg==0){ //a & b & c
                forfor=val-1;
            }
            for (int bp = 0; bp < forfor; bp++){
                // executeprocess(comds[bp],1,procarr,&numprocess,procarract,&numprocessact); // execute background process with flag 1 of bg 
                combinedexecute(comds[bp],1,procarr,&numprocess,procarract,&numprocessact,prevdir,homedir);
            }            
            if(onlybg==0){
                    combinedexecute(comds[val-1],0,procarr,&numprocess,procarract,&numprocessact,prevdir,homedir);
            }
          }   
        }
    }
    return 0;
}

