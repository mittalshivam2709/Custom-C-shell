#include "headers.h"

int DNS(char *home, char *ip) {
    struct hostent *he;
    struct in_addr **addr_list;
    if ((he = gethostbyname(home)) == NULL) {
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **)he->h_addr_list;
    if (addr_list[0] == NULL) {
        fprintf(stderr, "Unable to resolve IP address.\n");
        return 1;
    }
    strcpy(ip, inet_ntoa(*addr_list[0]));
    return 0;
}

void printmanpage(char* str){
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
    char host[]="man.he.net";
    char ip[1024];
    int val=DNS(host,ip);
    if (val!= 0) {
        return ;
    }
    // char host[4096];
    // snprintf(host,sizeof(host),"GET http://man.he.net/?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n",argv[1]);
    int socketfd;
    struct sockaddr_in server;
    char request[1024*1024];

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        perror("socket");
        return;
    }


    // Initialize server address struct
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    val=inet_pton(AF_INET, ip, &(server.sin_addr));
    if(val<=0) {
        perror("inet_pton");
        return ;
    }

    val=connect(socketfd, (struct sockaddr *)&server, sizeof(server));
    if (val< 0) {
        perror("connect");
        return ;
    }

    snprintf(request,sizeof(request),"GET http://man.he.net/?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n",commands[1]);

    // printf("here");
    val=write(socketfd, request, strlen(request)) ;
    if (val<0) {
        perror("write");
        return;
    }

    // Read and print the response
    char response[1024*1024];
    int n= read(socketfd, response, sizeof(response) - 1);
    int flag=0;
    while (n > 0) {
        response[n] = '\0';
        int i=0;
        if(flag==0){
            while(response[i]!='N' ||response[i+1]!='A'||response[i+2]!='M'||response[i+3]!='E'){
                flag=1;
                i++;
            }
            i--;
        }
        char ch=response[i];
        
        while(i!=n){
            ch=response[++i];
            while(ch!='<'&&i!=n){
                printf("%c",ch);
                ch=response[++i];
            }
            while(ch!='>'&& i!=n){
                ch=response[++i];
            }
        }
        n= read(socketfd, response, sizeof(response) - 1);
    }

    if (n < 0) {
        perror("read");
        return ;
    }

    // Close the socket
    close(socketfd);

}