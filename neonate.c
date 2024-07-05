#include "headers.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
*/

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void neonate(char* str){
    char* commands[1024];
    char newstr[1024];
    strcpy(newstr,str);
    char *token = strtok(newstr, " \t");
    int k=0;
    while (token!=NULL){
        commands[k]=token;
        k++;
        token=strtok(NULL," \t");
    }
    if(k!=3){
        printf("Invalid number of arguments\n");
        return;
    }
    int val=atoi(commands[2]);
    enableRawMode();
    setbuf(stdout, NULL);   
    int pid=fork();

    // child process prints the pid of the newly created process until the parent process kills it
    if(pid==0){
        while(1){
            FILE* fp=fopen("/proc/sys/kernel/ns_last_pid","r");
            char buffer[100];
            fgets(buffer,sizeof(buffer),fp);
            printf("%s",buffer);    
            sleep(val);
        }
    }

    // parent process waits for the user to press x and then kills the child process
    else if(pid>0){
        char c;
        setbuf(stdout, NULL);
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if(c=='x'){
                kill(pid,9);
                disableRawMode();
                return;
            }
        }
    }
}
