#ifndef _EXTRAFNS_H
#define _EXTRAFNS_H


int numofcommands(char* str);
void executeprocess(char* str,int bg,int* arr,int* idx);
char** seperateands(char* str);
void removespacesandtabs(char** comds,int num);

#endif