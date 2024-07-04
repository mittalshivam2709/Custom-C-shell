#include "headers.h"
void executeseek(char *path, char *target,int df,int ef,int ff, char** files,int *idx)
{
    DIR *dir;
    struct dirent *file_path_struct;
    struct stat fileStat;
    
    dir = opendir(path);
    if (dir) {
        while ((file_path_struct = readdir(dir)) != NULL) {
            if (strcmp(file_path_struct->d_name, ".") == 0 || strcmp(file_path_struct->d_name, "..") == 0) {
                continue; 
            }
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", path, file_path_struct->d_name);
            if (stat(filePath, &fileStat) == 0) {
                // printf("%s\n",file_path_struct->d_name);
                if ( strcmp(file_path_struct->d_name,target)!=0) {
                    // Recursively search in subdirectories
                    if(S_ISDIR(fileStat.st_mode)){
                        // printf("dir: %s\n",filePath);
                        executeseek(filePath, target,df,ef,ff,files,idx);
                    }
                } else if (strcmp(file_path_struct->d_name, target) == 0){
                    strcpy(files[(*idx)++],filePath);
                }
            }
        }
        closedir(dir);
    }
}

void seek(char *str, char *prevdir, char *homedir){
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
    char **files;
    files = (char **)malloc(sizeof(char *) * 1024);
    for (int j = 0; j < 1024; j++){
        files[j] = (char *)malloc(sizeof(char) * 1024);
    }
    int idx=0;
    executeseek(path, target,df,ef,ff,files,&idx);
    int file_count = 0;
    for (int j = 0; j < idx; j++)    {
        // printf("%s\n",files[j]);
        struct stat file_info;
        if (stat(files[j], &file_info) == 0){
            if (ef == 0){
                if (df == 0 && ff == 0){
                    printf("%s\n", files[j]);
                    file_count = 1;
                }
                else if (df && S_ISDIR(file_info.st_mode)){
                    printf("%s\n", files[j]);
                    file_count = 1;
                }
                else if (ff && S_ISREG(file_info.st_mode)){
                    printf("%s\n", files[j]);
                    file_count = 1;
                }
            }
            else{
                if (idx != 1){
                    break;
                }
                if (access(files[j], R_OK | X_OK) != 0){
                    printf("Missing permissions for task!\n");
                }
                else{
                    if (S_ISDIR(file_info.st_mode)){
                        chdir(files[j]);
                    }
                    else{
                        if (ff == 1 || (df == 0 && ff == 0)){
                            char *file_content = (char *)malloc(sizeof(char) * 1024);
                            FILE *f_c = fopen(files[j], "r");
                            while (fgets(file_content, 1024, f_c)){
                                printf("%s\n", file_content);
                                file_count = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    if (file_count == 0){
        printf("No match found!\n");
    }

}
