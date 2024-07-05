#ifndef _PROCLORE_H
#define _PROCLORE_H

struct Processinfo
{
    int pid;
    char status;
    int groupnum;
    int memory_occupied;
    char* executable_path;
};

void proclore(char* str);
int count (char* str);
int finddigits(int val);
char* converttostring(int num);
#endif