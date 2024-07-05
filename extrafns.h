#ifndef _EXTRAFNS_H
#define _EXTRAFNS_H

int numofcommands(char* str);
void executeprocess(char* str,int bg,struct allprocesses* procarr,int* numprocess,struct allprocesses* procarract,int* numprocessact);
char** seperateands(char* str);
int removespacesandtabs(char** comds,int num);
void combinedexecute(char* str,int bg,struct allprocesses* procarr,int* numprocess,struct allprocesses* procarract,int* numprocessact,char* prevdir,char* homedir);

#endif