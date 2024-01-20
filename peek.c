#include "headers.h"


int comparator(const void* str1,const void* str2){
    return strcmp(*(const char** )str1,*(const char**)str2);
}

// handle the case when invalid argument is given 
void peek(char* str){
    str+=4;
    int lf=0;
    int af=0;
    char* token=strtok(str," ");
    char* path=NULL;
    while(token!=NULL){
        // printf("here ");
        if(token[0]=='-'){
            // printf("this ");
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
        }
        else{
            path=token;
        }
        token=strtok(NULL," ");
    }    
    
    if(path==NULL){
        path=".";
    }
    char pwd[1024];
    getcwd(pwd,sizeof(pwd));
    chdir(path);
    char* temp=(char*)malloc(sizeof(char)*2);
    temp[0]='.';temp[1]='\0';
    DIR* directory=opendir(temp);
    if(directory==NULL){
        printf("Invalid Inputs !\n");return;
    }
    struct dirent* ptr=readdir(directory);
    char* names[1024];
    int num=0;

    while(ptr!=NULL){
        // printf("here");
        //  if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
        //     char full_path[1024];
        //     snprintf(full_path, sizeof(full_path), "%s/%s", path, ptr->d_name);

        struct stat info;
        int val=stat(ptr->d_name,&info);
        if(val==0){
            if((ptr->d_name)[0]=='.'){
                if(af==1){
                    names[num++]=ptr->d_name;
                }
            }
            else{
                names[num++]=ptr->d_name;
                // printf("read %s ",ptr->d_name);;
            } 
        // }
        }
        ptr=readdir(directory);
    }
    qsort(names,num,sizeof(const char*),comparator);
    for (int i = 0; i < num; i++)
    {
        // printf("yo");
        struct stat info;
        int val=stat(names[i],&info);
        if(val==0){

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
        }
    }
    closedir(directory);
    // printf("\n%s\n",path);
    // printf("store is %s\n ",storeddir);
    chdir(pwd);
    // char pwdvali[1024];
    // getcwd(pwdvali,sizeof(pwdvali));
    // printf(" %s ",pwdvali);
}


