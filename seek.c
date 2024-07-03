#include "headers.h"

// handle cases od showing error msg when incorrect input is given
void executeseek(char *path, char *target, char *prevdir, char *homedir, int ff, int df, int ef, int flag)
{
    DIR *directory = opendir(path);
    if (directory == NULL)
    {
        printf("Missing permissions for the directory\n");
        return;
    }
    struct dirent *ptr = readdir(directory);
    struct dirent *ptr2 = readdir(directory);
    char *names[1024];
    int num = 0;
    if (df + ef + ff == 0)
    {
        df = 1;
        ff = 1;
    }
    if (ef == 0)
    {
        while (ptr != NULL)
        {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
            {
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, ptr->d_name);
                struct stat info;
                int val = stat(ptr->d_name, &info);
                if (val == 0){
                    if (S_ISDIR(info.st_mode)){
                        if (strstr(ptr->d_name, target)){
                            if (df == 1){
                                if (flag != 1){
                                    char pwd[1024];
                                    getcwd(pwd, sizeof(pwd));
                                    char *temp = (char *)malloc(sizeof(char) * 1024);
                                    for (int i = 0; i < strlen(pwd); i++){
                                        temp[i] = pwd[i];
                                    }
                                    temp[strlen(pwd)] = '\0';
                                    if (strstr(pwd, homedir)){
                                        temp += strlen(homedir);
                                    }
                                    printf(".%s/", temp);
                                }
                                if (flag == 1){
                                    printf("./");
                                }
                                printf("%s\n", ptr->d_name);
                                executeseek(full_path, target, prevdir, homedir, ff, df, ef, 0);
                            }
                            if (ff == 1){
                                printf("./%s\n", ptr->d_name);
                            }
                        }
                        else
                        {
                            executeseek(full_path, target, prevdir, homedir, ff, df, ef, 0);
                        }
                    }
                    if (strstr(ptr->d_name, target))
                    {
                        if (ff == 1)
                        {
                            if (flag != 1)
                            {
                                char pwd[1024];
                                getcwd(pwd, sizeof(pwd));
                                char *temp = (char *)malloc(sizeof(char) * 1024);
                                for (int i = 0; i < strlen(pwd); i++)
                                {
                                    temp[i] = pwd[i];
                                }
                                temp[strlen(pwd)] = '\0';
                                if (strstr(pwd, homedir))
                                {
                                    temp += strlen(homedir);
                                }
                                printf(".%s/", temp);
                            }
                            printf("./%s\n", ptr->d_name);
                        }
                    }
                }
            }
            ptr = readdir(directory);
        }
        return;
    }
    else
    {
        int countc = 0;
        while (ptr != NULL)
        {
            if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
            {
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, ptr->d_name);
                struct stat info;
                int val = stat(ptr->d_name, &info);
                if (val == 0)
                {
                    if (strstr(ptr->d_name, target))
                    {
                        countc++;
                    }
                }
            }
            ptr = readdir(directory);
        }
        if (countc != 1)
        {
            return;
        }
        else
        {
            while (ptr2 != NULL)
            {
                if (strcmp(ptr2->d_name, ".") != 0 && strcmp(ptr2->d_name, "..") != 0)
                {
                    char full_path[1024];
                    snprintf(full_path, sizeof(full_path), "%s/%s", path, ptr2->d_name);
                    struct stat info;
                    int val = stat(ptr2->d_name, &info);
                    if (val == 0)
                    {
                        if (strstr(ptr2->d_name, target))
                        {
                            if (S_ISDIR(info.st_mode))
                            {
                                if (df == 1)
                                {
                                    printf("%s\n", ptr2->d_name);
                                }
                                chdir(ptr2->d_name);
                                if (ff == 1)
                                {
                                    printf("found directory with this name but not a file\n");
                                    return;
                                }
                            }
                            if (ff == 1)
                            {
                                printf("%s\n", ptr2->d_name);
                            }
                            if (info.st_mode & S_IRUSR)
                            {
                                FILE *fp = fopen(ptr2->d_name, "r");
                                if (fp == NULL)
                                {
                                    perror("Error opening file");
                                    return;
                                }
                                char buf[1024];
                                while (fgets(buf, sizeof(buf), fp) != NULL)
                                {
                                    printf("%s", buf);
                                }
                                fclose(fp);
                            }
                            else
                            {
                                printf("Missing permissions for the task\n");
                            }
                        }
                    }
                }
                ptr2 = readdir(directory);
            }
        }
    }
}

void seek(char *str, char *prevdir, char *homedir)
{
    str += 4;

    int ef = 0;
    int ff = 0;
    int df = 0;
    char newstr[1024];
    strcpy(newstr, str);
    char *token = strtok(newstr, " \t");
    char *path = NULL;
    char *target = NULL;
    while (token != NULL)
    {
        // printf("here ");
        if (token[0] == '-')
        {
            // printf("this ");
            if (strlen(token) == 2 && token[1] == 'd')
            {
                df = 1;
            }
            else if (strlen(token) == 2 && token[1] == 'e')
            {
                ef = 1;
            }
            else if (strlen(token) == 2 && token[1] == 'f')
            {
                ff = 1;
            }
            else if (strlen(token) == 3)
            {
                if (strcmp(token, "-ef") == 0 || strcmp(token, "-fe") == 0)
                {
                    ef = 1;
                    ff = 1;
                }
                else if (strcmp(token, "-ed") == 0 || strcmp(token, "-de") == 0)
                {
                    df = 1;
                    ef = 1;
                }
                else if (strcmp(token, "-df") == 0 || strcmp(token, "-fd") == 0)
                {
                    printf("Invalid flags !\n");
                    return;
                }
            }
            else if (strlen(token) == 4 && (strcmp(token, "-efd") == 0 || strcmp(token, "-edf") == 0 || strcmp(token, "-dfe") == 0 || strcmp(token, "-def") == 0 || strcmp(token, "-fde") == 0 || strcmp(token, "-fed") == 0))
            {
                printf("Invalid flags !\n");
                return;
            }
        }
        else
        {
            if (target == NULL)
            {
                target = token;
            }
            else
            {
                // printf("\ninside path\n");
                path = token;
            }
        }
        token = strtok(NULL, " \t");
        // printf("%s ",token);
    }
    if (df == 1 && ff == 1)
    {
        printf("Invalid flags!\n");
        return;
    }
    if (path == NULL)
    {
        path = ".";
    }
    executeseek(path, target, prevdir, homedir, ff, df, ef, 1);
}

// void seek(char* str,char* prevdir,char* homedir){
//     str+=4;
//     int ef=0;
//     int ff=0;
//     int df=0;
//     char* token=strtok(str," ");
//     char* path=NULL;
//     char* target=NULL;
//     while(token!=NULL){
//         // printf("here ");
//         if(token[0]=='-'){
//             // printf("this ");
//             if(strlen(token)==2&& token[1]=='d'){
//                     df=1;
//             }
//             if(strlen(token)==2&&token[1]=='e'){
//                 ef=1;
//             }
//             if(strlen(token)==2&&token[1]=='f'){
//                 ff=1;
//             }
//         }
//         else{
//             if(target==NULL){
//                 target=token;
//             }
//             else{
//                 // printf("\ninside path\n");
//                 path=token;
//             }
//         }
//         token=strtok(NULL," ");
//         // printf("%s ",token);
//     }

//     if(df==1 && ff==1){
//         printf("Invalid flags!\n");
//         return;
//     }
//     if(path==NULL){
//         path=".";
//     }

//     // char newpath[strlen(path)+5];
//     // newpath[0]='w';newpath[1]='a';newpath[2]='r';newpath[3]='p';newpath[4]=' ';
//     // for (int i = 5; i < strlen(path)+5; i++){
//     //     newpath[i]=path[i-5];
//     // }
//     // printf("%s\n",newpath);
//     // printf("%c ",path[strlen()])
//     // char store[1024];
//     // getcwd(store,sizeof(store));
//     // warp(newpath,prevdir,homedir);
//     // char this[1024];
//     // getcwd(this,sizeof(this));
//     // printf("%s\n",this);
//     if(ef==0){
//         DIR* directory=opendir(path);
//         struct dirent* ptr=readdir(directory);
//     // char* names[1024];
//     // int num=0;

//     while(ptr!=NULL){
//         // printf("here");
//         struct stat info;
//         int val=stat(ptr->d_name,&info);
//         if(val==0){
//             int isdir=S_ISDIR(info.st_mode);
//             if(isdir&& ff==0){
//                 if(strcmp(ptr->d_name,target)==0){
//                     printf("./%s ",ptr->d_name);
//                 }

//             }
//             if(!isdir && df==0){
//                 if(strcmp(ptr->d_name,target)==0){
//                     printf("./%s ",ptr->d_name);
//                 }
//             }

//         }
//         ptr=readdir(directory);
//     }
//     }
//     // add string warp to store also
//     // warp(store,prevdir,homedir);
// }
