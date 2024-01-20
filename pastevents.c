#include "headers.h"

void executepastevents(char *str,char* path,char* homedir,char* prevdir,int* idx)
{
    char pastevents[11]="pastevents";

    if (strstr(str , "pastevents") != NULL && strstr(str , "purge"))
    {
        // empty the file also
        FILE *file = fopen(path, "w");
        if (file == NULL)
        {
            perror("Error opening the history file");
        }
        fclose(file);
    }
    else if (strcmp(str , "pastevents") == 0)
    {
        // if(Q->numelems==15){
        // Print(Q);
        // you may clear the file also
        FILE *fp = fopen(path, "r");
        if (fp == NULL)
        {
            perror("Error opening file");
            return ;
        }
        long long int linenum = 0;
        fseek(fp, 0, SEEK_END);
        long long int file_size = ftell(fp);
        long long int pos = file_size;
        fseek(fp, 0, SEEK_SET);
        char line[1024];

        // Start from the end and find the last 15 lines
        // printf("yo %lld %lldyo",linenum,pos);
        while (linenum < 17 && pos > 0)
        {
            // printf("padharo");
            fseek(fp, --pos, SEEK_SET);
            // printf("%c",fgetc(fp));
            if (fgetc(fp) == '\n')
            {
                linenum++;
            }
        }
        // printf(" %lld ",linenum);
        int count = 0;
        while (count < 16 && fgets(line, sizeof(line), fp) != NULL)
        {
            // printf(":innns");
            printf("%s", line);
            count++;
        }
        fclose(fp);
        // printf("once more");
    }
    // }
    else if (strstr(str , "pastevents") != NULL && strstr(str , "execute") != NULL)
    {
        // execute the command
        FILE *fp = fopen(path, "r");
        if (fp == NULL)
        {
            perror("Error opening file");
            return ;
        }
        long long int linenum = 0;
        fseek(fp, 0, SEEK_END);
        long long int file_size = ftell(fp);
        long long int pos = file_size;
        fseek(fp, 0, SEEK_SET);
        char line[1024];

        // Start from the end and find the last 15 lines
        // printf("yo %lld %lldyo",linenum,pos);
        while (linenum < 17 && pos > 0)
        {
            // printf("padharo");
            fseek(fp, --pos, SEEK_SET);
            // printf("%c",fgetc(fp));
            if (fgetc(fp) == '\n')
            {
                linenum++;
            }
        }
        while(fgetc(fp)!='\n'){
        fseek(fp, ++pos,SEEK_SET);
        }
            

        // printf(" %lld ",linenum);
        int count = 0;
        char arr[17][1024];
        // for (int givem = 0; givem < 17; givem++)
        // {
        //     arr[givem]=(char*)malloc(sizeof(char)*1024);
        // }
        // char line[1/024];
        int arridx = 0;
        while (count < 16 && fgets(line, sizeof(line), fp) != NULL)
        {
            // printf(":innns");
            // printf("%s", line);
            char newline[1024];
            for (int p = 0; p < strlen(line) + 1; p++)
            {
                newline[p] = line[p];
            }
            // printf("%s",newline);
            strcpy(arr[arridx], newline);
            arridx++;
            count++;
        }
        // printf("%d ", count);
        strcpy(arr[count], "khatam");
        
        int index = -1;
        for (int z = 0; z < 16; z++)
        {
            // printf("%s ", arr[i]);
            if (strcmp(arr[z], "khatam") == 0)
            {
                index = z;
                break;
            }
        }
        str[strlen( str) - 1] = '\0'; // \n replaced by \0

        if (strcmp( str, "exit") == 0) {
            // break;
            return;
        }

        char* commands[1024];
        char *token = strtok( str, ";");
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
                // executeprocess(comds[bp],1,arr,&idx);
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
                    // executepastevents(comds[val-1],path,homedir,prevdir,&idx);
                }
                else if(strstr(comds[val-1],"proclore")!=NULL){
                    proclore(comds[val-1]);
                }
            }
            // if not then execute the process using execvp
            else{
                // printf("this is your boy ");
                // executeprocess(comds[val-1],0,arr,&idx);
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
        
        // printf("%d",index);
        fclose(fp);
    }
}
