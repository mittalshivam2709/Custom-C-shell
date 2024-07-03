#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <termios.h>

extern int fgpid;
extern int ctrlzflag;
extern char path[1024];

struct allprocesses
{
    char procid[100];
    char procname[100];    
    int flag;
};


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAX_ARGS 64

#include "peek.h"
#include "warp.h"
#include "seek.h"
#include "queue.h"
#include "proclore.h"
#include "prompt.h"
#include "pastevents.h"
#include "extrafns.h"
#include "ioredirection_pipe.h"
#include "activities.h"
#include "ping.h"
#include "fgbg.h"
#include "iman.h"
#include "neonate.h"


#endif
