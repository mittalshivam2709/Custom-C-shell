#include "headers.h"

int numofcommands(char* str){
    char temp[strlen(str)+1];
    for (int i = 0; i < strlen(str); i++){
        temp[i]=str[i];
    }
    temp[strlen(str)]='\0';
    char* token=strtok(temp,"&");
    int num=0;
    while (token!=NULL){
        num++;
        token=strtok(NULL,"&");
    }
    return num;
}

char** seperateands(char* str){
    int val=numofcommands(str);
    char** retcommands=(char**)malloc(sizeof(char*)*(val));
    char* token=strtok(str,"&");
    int i=0;
    while (token!=NULL)
    {
        retcommands[i++]=token;
        token=strtok(NULL,"&");
    }
    return retcommands;
}

int removespacesandtabs(char** comds,int num){
    int isexit=-1;
    for (int cn = 0; cn < num; cn++) // cn is command no.
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
            if(strcmp(finalstr,"exit")==0){
                isexit=cn;
            }
        }   
    }    
    return isexit;
}

// a & b& c& d
// fork ()
// 2 proc
// child execute
// parent store pid


void combinedexecute(char* str,int bg,struct allprocesses* procarr,int* numprocess,struct allprocesses* procarract,int* numprocessact,char* prevdir,char* homedir){
        if (strstr(str,"warp")){               
            warp(str,prevdir,homedir);
        }
        else if(strlen(str)>=4&&strstr(str,"peek")){
            peek(str,prevdir,homedir);
        }    
        else if(strlen(str)>=4&&strstr(str,"seek")){
            seek(str,prevdir,homedir);
        }
        else if(strstr(str,"pastevents")!=NULL){
            executepastevents(str,homedir,prevdir,procarr,numprocess,procarract,numprocessact);
        }
        else if(strstr(str,"proclore")!=NULL){
            proclore(str);
        }
        else if(strcmp(str,"activities")==0){
            // printf("inside");
            activities(str,procarract,numprocessact);
        }
        else if(strstr(str,"ping")){
            ping(str,procarract,numprocessact);
        }
        else if( strstr(str,"fg")||strstr(str,"bg")){
            fgbg(str,procarract,numprocessact);
        }
        else if(strstr(str,"iMan")){
            printmanpage(str);
        }
        else if(strstr(str,"neonate")){
            neonate(str);
        }
        // if not then execute the process using execvp but send flag of being background as 0
        else{
            executeprocess(str,bg,procarr,numprocess,procarract,numprocessact);
            // executeprocess(str,0,procarr,&numprocess,procarract,&numprocessact);
        }
}

void executeprocess(char* str,int bg,struct allprocesses* procarr,int* numprocess,struct allprocesses* procarract,int* numprocessact){

    int status;
    
    char newstr[1024];
    strcpy(newstr,str);
    
    char* commands[1024];
    int k=0;
    char *token = strtok(newstr, " <>\t");
    while (token!=NULL){
        commands[k]=(char*)malloc(sizeof(char)*(strlen(token)+10));
        strcpy(commands[k],token);
        k++;
        token=strtok(NULL," <>\t");
    }
    commands[k]=NULL;
    int pid=fork();
    if(pid==0){
            fflush(stdout);
            // if(strstr(str,"sed")){
            //     char* cmd[2];
            //     cmd[0]=str;
            //     cmd[1]=NULL;
            //     execvp(cmd[0],cmd);
            //     perror("execvp");
            //     exit(EXIT_FAILURE);
            // }else{
            // printf("%s\n",str);
            // char* cmd[2];
            // cmd[0]="sh";
            // cmd[1]="-c"; 
            setpgid(0,0);
            // cmd[0]=str;
            // cmd[1]=NULL;
            // if(commands[0]=="sed"){

            // }else{
                execvp(commands[0],commands);
                perror("execvp");
                exit(EXIT_FAILURE);
            // }
    }
    else if(pid>0){
        if(!bg){
            // wait since this is a foreground process

            fgpid=pid;

            char* tempid=converttostring(pid);
            strcpy(procarr[(*numprocessact)].procname,str);

            strcpy(procarr[(*numprocessact)].procid,tempid);
            (*numprocessact)++;
            while(1){
                
                int status2;
                int rr=waitpid(pid,&status2,WUNTRACED);
                if(WIFSTOPPED(status2)){
                    procarr[(*numprocess)].flag=0;
                    strcpy(procarr[(*numprocess)].procname,str);
                    strcpy(procarract[(*numprocessact)].procname,str);
                    strcpy(procarr[(*numprocess)].procid,tempid);
                    strcpy(procarract[(*numprocessact)].procid,tempid);
                    (*numprocess)++;
                    (*numprocessact)++;
                    break;
                }
                if(rr>0 || WIFEXITED(status2)){
                    break;
                }
            }
        }
        else{
            printf("%d\n",pid);            //getpid();
            // arr[(*idx)++]=pid;
            char* tempid=converttostring(pid);

            procarr[(*numprocess)].flag=0;
            strcpy(procarr[(*numprocess)].procname,str);
            strcpy(procarract[(*numprocessact)].procname,str);
            strcpy(procarr[(*numprocess)].procid,tempid);
            strcpy(procarract[(*numprocessact)].procid,tempid);
            (*numprocess)++;
            (*numprocessact)++;
        }
        
    }
}

