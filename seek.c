#include "headers.h"

// handle cases od showing error msg when incorrect input is given

void seek(char* str,char* prevdir,char* homedir){
    str+=4;
    int ef=0;
    int ff=0;
    int df=0;
    char* token=strtok(str," ");
    char* path=NULL;
    char* target=NULL;
    while(token!=NULL){
        // printf("here ");
        if(token[0]=='-'){
            // printf("this ");
            if(strlen(token)==2&& token[1]=='d'){
                    df=1;
            }
            else if(strlen(token)==2&&token[1]=='e'){
                ef=1;
            }
            else if(strlen(token)==2&&token[1]=='f'){
                ff=1;
            }
            else if(strlen(token)==3){
                if(strcmp(token,"-ef")==0 || strcmp(token,"-fe")==0){
                    ef=1;
                    ff=1;
                }
                else if(strcmp(token,"-ed")==0 ||strcmp(token,"-de")==0){
                    df=1;ef=1;
                }
                else if(strcmp(token,"-df")==0||strcmp(token,"-fd")==0){
                    printf("Invalid flags !\n");
                    return;
                }
            }
            else if(strlen(token)==4 && (strcmp(token,"-efd")==0 ||strcmp(token,"-edf")==0||strcmp(token,"-dfe")==0||strcmp(token,"-def")==0 || strcmp(token,"-fde")==0 || strcmp(token,"-fed")==0)){
                printf("Invalid flags !\n");return;
            }
        }
        else{
            if(target==NULL){
                target=token;
            }
            else{
                // printf("\ninside path\n");
                path=token;
            }
        }
        token=strtok(NULL," ");
        // printf("%s ",token);
    }
   
    if(df==1 && ff==1){
        printf("Invalid flags!\n");
        return;
    }
    if(path==NULL){
        path=".";
    }

    DIR* directory=opendir(path);
    struct dirent* ptr=readdir(directory);
    char* names[1024];
    int num=0;
    if(df+ef+ff==0){
        df=1;
        ff=1;
    }
   
    while(ptr!=NULL){
        // printf("here");
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, ptr->d_name);

            struct stat info;
            int val=stat(ptr->d_name,&info);
            if(val==0){
                if((ptr->d_name)[0]!='.'){
                    names[num++]=ptr->d_name;
                    printf("read %s ",ptr->d_name);;
                } 
            }
        }
        ptr=readdir(directory);
    }

    if(ef==0){
    for (int i = 0; i < num; i++)
    {
        // *******************update the following condition 
        if(strstr(names[i],target)){
            struct stat info;
            int val=stat(names[i],&info);
            if(val==0){
                if(S_ISDIR(info.st_mode)){
                    // **************check the correctness of the following
                    if(df==1){
                        printf("./%s\n",names[i]);
                    }
                    // *************change path before calling seek
                    seek(str,prevdir,homedir);
                }
                else{
                    // ****************check the correctness of the following 
                    if(ff==1){
                        printf("./%s\n",names[i]);
                    }
                }
            }
        }
        else{
            struct stat info;
            int val=stat(names[i],&info);
            if(val==0){
                if(S_ISDIR(info.st_mode)){
                    // ******change path before calling seek
                    seek(str,prevdir,homedir);
                }
            }
        }
    }
    //*************write the case of no match found also 
    }
    else{
        int count =0;
        for (int i = 0; i < num; i++)
        {
            if(strstr(names[i],target)){
                count ++;
            }
        }
        if(count>=2){
            if(ff==0&&df==0){
                return;
            }
            else if(df==1){
                for (int i = 0; i < num; i++)
                {
                    if(strstr(names[i],target)){
                        struct stat info;
                        int val=stat(names[i],&info);
                        if(val==0){
                            if(S_ISDIR(info.st_mode)){
                                // change the path and homedir 
                                }
                    }
                }
                
            }
        }
    }
    // add string warp to store also 
    // warp(store,prevdir,homedir);
}
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


