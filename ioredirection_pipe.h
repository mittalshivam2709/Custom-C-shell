#ifndef _IOREDIRECTION_H
#define _IOREDIRECTION_H

void io_pipe_handling(char **pipesep, int num, struct allprocesses *procarr, int *numprocess, struct allprocesses *procarract, int *numprocessact,char* prevdir,char* homedir);
int io_redirect(char* iosepcomm);

#endif