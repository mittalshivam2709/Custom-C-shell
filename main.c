#include "headers.h"


// execute all the process in the child process and if foreground process is being executed then use a wait in parent for the same chidl
// print the output any time it does not matter however print the exit status just after the next prompt
// therefore store the pid in parent

int main() {
    char prevdir[1024];
    char homedir[1024];
    char* prompthomedir=(char*)malloc(sizeof(char)*1024);
    // char prompthomedir[1024];
    int i=0;
    Queue Q=NULL;
    Q = Init(Q);
    char* store[1024];
    int arr[1024];
    int idx=0;
    while (1) {
        // infinte running loop 
        // printf("very good");
        // call prompt as required
        // sleep(2);
        



        idx=0;
        if(i==0){
            prompthomedir=getcwd(prompthomedir,1024);
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
      
        char input[1024];
        fgets(input, sizeof(input), stdin);

        char pastevents[11]="pastevents";
            char path[1024];
            strcpy(path,homedir);
            strcat(path,"/history.txt");
        
        if(strstr(input,pastevents)==NULL){
            // printf("null hai ");
            Pushback(Q,input);
            if(Q->numelems==16){
                Popfront(Q);
            }
            // printf("here");
            printf(" %s ",path);
            FILE *file = fopen(path, "a");
            if (file==NULL) {
                perror("Error opening the history file");
            } 
            // printf("%s ",input);
            fprintf(file, "%s", input);
            fclose(file);
        }

        
        input[strlen(input) - 1] = '\0'; // \n replaced by \0

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char* commands[1024];
        char *token = strtok(input, ";");
        int k=0;
 
        while (token!=NULL)
        {
            commands[k]=token;
            k++;
            token=strtok(NULL,";");
        }

        //seperated by ; and stored in commmands which is array of pointers
        // while (token != NULL) {
        // int idx=-1;

        for (int j = 0; j < k; j++)
        {
            int onlybg=0;
            int lastidx=strlen(commands[j])-1;

            // check if u have only &


            // for (int a = strlen(commands[j])-1; a >=0 ; a--){
            //     if(commands[j][a]==' '|| commands[j][a]=='\t'){
            //         lastidx--;
            //     }
            //     else{
            //         if(commands[j][a]=='&'){
            //             onlybg=1;
            //         }
            //         break;
            //     }
            // }
            // commands[j][lastidx]='\0';
            // printf(" %c %c ",commands[j][lastidx-1],commands[j][lastidx]);
            // multiple command having spaces and tabs and & in between 
            int val=numofcommands(commands[j]);
            // printf("%d",val);






            // formed array of strings which does not have & in between and seperated by spaces and tabs
            char** comds=seperateands(commands[j]); 
            // cmds has val num of commands
            // now remove spaces and tabs from each of the commands
            removespacesandtabs(comds,val);
            // now in comds[i] which has size val we have the actual command that needs to be executed
            // the spaces in between will be taken care by strtok of individual cmd

            // function call for the background process execution
            int forfor=val;
            if(onlybg==0){
                forfor=1;
            }
            // printf("here");
            for (int bp = 0; bp < forfor; bp++)
            {
                executeprocess(comds[bp],1,arr,&idx);
            }
            // check if the foreground process is one among implementation done by us
            if(onlybg==0){
            if(strstr(comds[val-1],"warp") || strstr(comds[val-1],"peek")|| strstr(comds[val-1],"seek")|| strstr(comds[val-1],"pastevents")|| strstr(comds[val-1],"proclore"))
            {
                // printf("aaiye");
                // printf("here we go ");
                if (strstr(comds[val-1],"warp"))
                {               
                    warp(comds[val-1],prevdir,homedir);
                }   
                else if(strlen(comds[val-1])>=4&&strstr(comds[val-1],"peek")){
                    peek(comds[val-1]);
                }    
                else if(strlen(comds[val-1])>=4&&strstr(comds[val-1],"seek")){
                    seek(comds[val-1],prevdir,homedir);
                }
                else if(strstr(comds[val-1],pastevents)!=NULL)
                {
                    executepastevents(comds[val-1],path,homedir,prevdir,&idx);
                }
                else if(strstr(comds[val-1],"proclore")!=NULL){
                    proclore(comds[val-1]);
                }
            }
            // if not then execute the process using execvp
            else{
                // printf("this is your boy ");
                executeprocess(comds[val-1],0,arr,&idx);
            }
            }
            // clock_t end=clock();
            // double totaltime=(end-start)/CLOCKS_PER_SEC;
            // printf("%f ",totaltime);
            // if(totaltime>2){
            //     store[idx++]=commands[j];
            // }
        }   
        // printf("%s ",history[--idx]);
        // idx++;
        // fclose(file);
        // idx=0;
    }
    return 0;
}

