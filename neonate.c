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
    int val=atoi(commands[2]);
    enableRawMode();
    setbuf(stdout, NULL);   
    int pid=fork();
    if(pid==0){
        while(1){
            FILE* fp=fopen("/proc/sys/kernel/ns_last_pid","r");
            char buffer[100];
            fgets(buffer,sizeof(buffer),fp);
            printf("%s",buffer);    
            sleep(val);
        }
    }
    else if(pid>0){
        char c;
        // while (1) {
            setbuf(stdout, NULL);
            // int pt = 0;
            // int flag=0;
            while (read(STDIN_FILENO, &c, 1) == 1) {
                if(c=='x'){
                    // printf("x is preseed");
                    // flag=1;
                    kill(pid,9);
                    disableRawMode();
                    return;
                    // printf("Here");
                    break;
                }
            }
            // if(flag){
            //     // printf("yoyo");
            //     break;
            // }
            // printf("\nInput Read: [%s]\n", inp);
        // }        
    }
}
