#include "headers.h"

void executepastevents(char *str, char *homedir, char *prevdir, struct allprocesses *procarr, int *numprocess, struct allprocesses *procarract, int *numprocessact){
    char pastevents[11] = "pastevents";
    Queue Q = NULL;
    Q = Init(Q);
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        perror("Error opening file");
        return;
    }
    // extract last 15 commands

    long long int linenum = 0;
    fseek(fp, 0, SEEK_END); // pointer set to next to eof
    long long int file_size = ftell(fp);
    long long int pos = file_size;
    fseek(fp, 0, SEEK_SET);

    while (linenum < 16 && pos >= 0){
        fseek(fp, --pos, SEEK_SET);
        if (fgetc(fp) == '\n'){
            linenum++;
        }
    }
    if (linenum <= 15){
        fseek(fp, 0, SEEK_SET);
    }

    char line[1024];
    int count = 0;
    while (count < 16 && fgets(line, sizeof(line), fp) != NULL){
        char *temp = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(temp, line);
        Pushback(Q, temp);
        count++;
    }
    fclose(fp);

    if (strstr(str, "pastevents") && strstr(str, "purge")){
        // empty the file 
        FILE *file = fopen(path, "w");
        if (file == NULL){
            perror("Error opening the history file");
        }
        fclose(file);
        return;
    }

    else if (strcmp(str, "pastevents") == 0){
        Print(Q);return;
    }

    else if (strstr(str, "pastevents") && strstr(str, "execute")){
        char temp[strlen(str) + 1];
        for (int i = 0; i < strlen(str); i++){
            temp[i] = str[i];
        }
        temp[strlen(str)] = '\0';
        char *tken = strtok(temp, " \t");
        int val = 100;
        while (tken != NULL){
                val = tken[0] - '0';
            if (strlen(tken) == 1){
                break;
            }
            else if (strlen(tken) == 2){
                val = tken[0] - '0';
                val *= 10;
                val += (tken[1] - '0');
                break;
            }
            tken = strtok(NULL, " \t");
        }
        if (val > Q->numelems){
            printf("Index Out of range !!\n");
            printf("The number of commands in history are %d\n", Q->numelems);
            return;
        }

        char *input = findElem(Q, Q->numelems - val + 1);

        FILE *fp = fopen(path, "r");
        if (fp == NULL){
            perror("Error opening file");
            return ;
        }

        // store the last command in the history file
        fseek(fp, 0, SEEK_END); // pointer set to next to eof
        long long int file_size = ftell(fp);
        long long int pos = file_size;
        int linenum = 0;
        // printf("here is %c\n",fgetc(fp));
        while (linenum < 2 && pos >= 0){
            fseek(fp, --pos, SEEK_SET);
            if (fgetc(fp) == '\n'){
                linenum++;
            }
        }
        char line[1024];
        fgets(line, sizeof(line), fp);
        fclose(fp);

        // code from main.c to execute the command
        
        if (strstr(input, pastevents) == NULL && strcmp(input, line) != 0){
            FILE *file = fopen(path, "a");
            if (file == NULL){
                // printf("%s\n",path);
                perror("Error opening the history file");
            }
            fprintf(file, "%s", input);
            fclose(file);
        }

        input[strlen(input) - 1] = '\0'; // \n replaced by \0


        char *commands[1024];
        char *token = strtok(input, ";");
        int k = 0;

        while (token != NULL){
            commands[k] = token;
            k++;
            token = strtok(NULL, ";");
        }

        // seperated by ; and stored in commmands which is array of pointers
        // loop for commands free of semicolons
        for (int j = 0; j < k; j++){
            int onlybg = 0;
            // check if u have only &
            int a;
            for (a = strlen(commands[j]) - 1; a >= 0; a--){
                if (commands[j][a] == ' ' || commands[j][a] == '\t'){
                }
                else
                {
                    if (commands[j][a] == '&')
                    { // & at the last
                        onlybg = 1;
                    }
                    commands[j][a + 1] = '\0';
                    break;
                }
            }
            int pipe_or_io = 0;
            int f = 0;
            for (int p = 0; p < a + 1; p++)
            {
                if (commands[j][p] == '|')
                {
                    pipe_or_io = 1;
                }
                if (commands[j][p] == '<' || commands[j][p] == '>')
                {
                    f = 1;
                }
            }
            if (pipe_or_io && f)
            {
                char *token = strtok(commands[j], "|");
                char **pipesep = (char **)malloc(sizeof(char *) * 1024);
                int pipes = 0;
                while (token != NULL)
                {
                    pipesep[pipes++] = token;
                    token = strtok(NULL, "|");
                }
                io_pipe_handling(pipesep, pipes, procarr, numprocess, procarract, numprocessact, prevdir, homedir);
            }
            else if (f)
            {
                int storein = dup(STDIN_FILENO);
                int storeout = dup(STDOUT_FILENO);
                int val = io_redirect(commands[j]);
                char *cmd[4];
                char *tkn = strtok(commands[j], "<>");
                combinedexecute(tkn, 0, procarr, numprocess, procarract, numprocessact, prevdir, homedir);
                close(val);
                dup2(storein, STDIN_FILENO);
                dup2(storeout, STDOUT_FILENO);
            }
            else if (pipe_or_io)
            {

                char *token = strtok(commands[j], "|");
                char **pipesep = (char **)malloc(sizeof(char *) * 1024);
                int pipes = 0;
                while (token != NULL)
                {
                    pipesep[pipes++] = token;
                    // printf("%s\n",token);
                    token = strtok(NULL, "|");
                }

                io_pipe_handling(pipesep, pipes, procarr, numprocess, procarract, numprocessact, prevdir, homedir);
            }
            // printf("jere");
            else
            {
                // multiple command having spaces and tabs and & in between
                int val = numofcommands(commands[j]);

                // formed array of strings which does not have & in between and seperated by spaces and tabs
                char **comds = seperateands(commands[j]);

                // cmds has val num of commands
                // now remove spaces and tabs from each of the commands
                
                removespacesandtabs(comds, val);

                // now in comds[i] which has size=val we have the actual command that needs to be executed
                // the spaces in between will be taken care by strtok of individual cmd

                //************ now in comds

                // function call for the background process execution

                int forfor = val;
                if (onlybg == 0)
                { // a & b & c
                    forfor = val - 1;
                }
                for (int bp = 0; bp < forfor; bp++)
                {
                    // executeprocess(comds[bp],1,procarr,&numprocess,procarract,&numprocessact); // execute background process with flag 1 of bg
                    combinedexecute(comds[bp], 1, procarr, numprocess, procarract, numprocessact, prevdir, homedir);
                }
                if (onlybg == 0)
                {
                    combinedexecute(comds[val - 1], 0, procarr, numprocess, procarract, numprocessact, prevdir, homedir);
                }
            }
        }
 
        return;
    }
    else{
        printf("Invalid input\n");
    }
}
