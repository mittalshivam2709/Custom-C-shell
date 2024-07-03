#include "headers.h"


int comparator(const void* str1,const void* str2){
    return strcmp(*(char** )str1,*(char**)str2);
}

// handle the case when invalid argument is given 
void peek(char* str,char* prevdir,char* homedir){
    char* newstr=(char*)malloc(sizeof(char)*1024);
    strcpy(newstr,str);
    // to bypass peek
    newstr+=4;
    bool lf=0;
    bool af=0;
    // to store the path of the directory whose info is to be printed
    char* path=NULL;
    char* token=strtok(newstr," \t");
    while(token!=NULL){

        if(token[0]=='-'){
            if(strlen(token)==3 && (strcmp(token,"-al")==0 || strcmp(token,"-la")==0)){
                af=lf=1;
            }
            else if(strlen(token)==2){
                if(token[1]=='a'){
                    af=1;
                }
                else if(token[1]=='l'){
                    lf=1;
                }
            }
            else{
                path=token;
            }
        }
        else{
            path=token;   
        }
        token=strtok(NULL," \t");
    }    
    if(path==NULL){
        path=".";
    }

    // to restore it back to the original directory later
    char pwd[1024];
    getcwd(pwd,sizeof(pwd));

    // code similar to warp to change the directory
    token=strtok(path," \t/");
    while(token!=NULL){
        if(token[0]=='-'){
            chdir(prevdir);
            char pwd[1024];
            getcwd(pwd,sizeof(pwd));
        }
        else if(token[0]=='~'){
            chdir(homedir);
            char pwd[1024];
            getcwd(pwd,sizeof(pwd));
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
            }
        }
        token=strtok(NULL," \t/");
    }

    // . because we want to open the current directory
    char* temp=(char*)malloc(sizeof(char)*2);
    temp[0]='.';temp[1]='\0';

    DIR* directory=opendir(temp);
    if(directory==NULL){
        printf("Invalid Inputs !\n");return;
    }
    
    struct dirent* ptr=readdir(directory);

    // names store the names of the files in the directory
    char* names[1024];
    int num=0;

    while(ptr!=NULL){
    
        struct stat info;
        int val=stat(ptr->d_name,&info);
        if(val==0){
            if((ptr->d_name)[0]=='.'){
                // this means this is a hidden file hence,if flag is 1 then only we will print it
                if(af==1){
                    names[num++]=ptr->d_name;
                }
            }
            else{
                names[num++]=ptr->d_name;
            } 
        }
        ptr=readdir(directory);
    }

    qsort(names,num,sizeof(const char*),comparator);
    
    for (int i = 0; i < num; i++){
        struct stat info;
        int val=stat(names[i],&info);
        if(val==0){
            // display all the information if lf is set
            if(lf==1){
                if(S_ISDIR(info.st_mode)){
                    printf("d");
                }
                else{
                    printf("-");
                }
                // Owner's permissions
                printf((info.st_mode&S_IRUSR) ? "r":"-");
                printf((info.st_mode&S_IWUSR) ? "w":"-");
                printf((info.st_mode&S_IXUSR) ? "x":"-");
                printf((info.st_mode&S_IRGRP) ? "r":"-");
                printf((info.st_mode&S_IWGRP) ? "w":"-");
                printf((info.st_mode&S_IXGRP) ? "x":"-");
                printf((info.st_mode&S_IROTH) ? "r":"-");
                printf((info.st_mode&S_IWOTH) ? "w":"-");
                printf((info.st_mode&S_IXOTH) ? "x":"-");
                
                printf(" ");
                int count=0;
                if(info.st_mode&S_IXUSR){
                    count++;
                }
                if(info.st_mode&S_IXGRP){
                    count++;
                }
                if(info.st_mode&S_IXOTH){
                    count++;
                }
                //hardlink
                printf("%lu ",info.st_nlink);

                struct passwd *owner = getpwuid(info.st_uid);
                printf("%s ", owner->pw_name);


                struct group *group = getgrgid(info.st_gid);
                printf("%s ",group->gr_name);

                printf("%lld ",(long long int)info.st_size);

                char time[1024];
                strftime(time,sizeof(time),"%b %d %H:%M",localtime(&info.st_mtime));
                printf("%s ",time);

                if(S_ISDIR(info.st_mode)){
                    printf(ANSI_COLOR_BLUE "%s\n" ANSI_COLOR_RESET,names[i]);
                }
                else if(count==3){
                    printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,names[i]);
                }
                else{
                    printf("%s\n",names[i]);
                }
            }
            
            else{
                int count=0;
                if(info.st_mode&S_IXUSR){
                    count++;
                }
                if(info.st_mode&S_IXGRP){
                    count++;
                }
                if(info.st_mode&S_IXOTH){
                    count++;
                }
                // print in blue if it is a directory
                if(S_ISDIR(info.st_mode)){
                    printf(ANSI_COLOR_BLUE "%s\n" ANSI_COLOR_RESET,names[i]);
                }
                // print in green if it an executable
                else if(count==3){
                    printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,names[i]);
                }
                else{
                    printf("%s\n",names[i]);
                }
            }
        }
    }
    closedir(directory);
    // printf("\n%s\n",path);
    // printf("store is %s\n ",storeddir);
    chdir(pwd);

}


