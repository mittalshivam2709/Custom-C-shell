#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 4096

// Function to perform DNS resolution and return the IP addres
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <man_page_name>\n", argv[0]);
        return 1;
    }

    char *hostname = "man.he.net";
    // char hostname[4096];
    // snprintf(hostname,sizeof(hostname),"GET http://man.he.net/?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n",argv[1]);
    char ip_address[INET_ADDRSTRLEN];

    if (resolveDNS(hostname, ip_address) != 0) {
        return 1;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char request[MAX_BUFFER_SIZE];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, ip_address, &(server_addr.sin_addr)) <= 0) {
        perror("inet_pton");
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    // Create HTTP GET request
    // snprintf(request, sizeof(request), "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", argv[1], hostname);
    snprintf(request,sizeof(request),"GET http://man.he.net/?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n",argv[1]);


    // Send the HTTP request
    if (write(sockfd, request, strlen(request)) < 0) {
        perror("write");
        return 1;
    }

    // Read and print the response
    char response[MAX_BUFFER_SIZE];
    int n;
    int flag=0;
    while ((n = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[n] = '\0';
        // int i=-1;    
        // char ch;
        // int flag=0;
        // while(i!=n){
        //     ch=response[++i];
        //     int idx=0;
        //     char line[1024];
        //     while(ch!='\n'){
        //         line[idx++]=response[i];
        //         ch=response[++i];
        //     }
        //     line[idx]='\0';
        //     int store=idx;
        //     if(flag==0){
        //         if(strstr(line,"NAME")){
        //             flag=1;
        //             idx=-1;
        //             while(idx!=store){
        //                 ch=line[++idx];
        //                 while(ch!='<'){
        //                     printf("%c",ch);
        //                     ch=line[++idx];
        //                 }
        //                 while(ch!='>'){
        //                     ch=line[++idx];
        //                 }
        //             }
        //         }
        //         else{
        //             printf("here\n");
        //             continue;
        //         }
        //     }
        //     else{
        //         idx=0;
        //         while(idx!=store){
        //             while(ch!='<'){
        //                 printf("%c",ch);
        //                 ch=line[++idx];
        //             }
        //             while(ch!='>'){
        //                 ch=line[++idx];
        //             }
        //         }
        //     }
        // }
        int i=0;
        if(flag==0){
            while(response[i]!='N' ||response[i+1]!='A'||response[i+2]!='M'||response[i+3]!='E'){
                flag=1;
                i++;
            }
            i--;
        }
        char ch=response[i];
        // for (int j = 0; j < 1000; j++)
        // {
        //     printf("%c",ch);
        // }
        
        while(i!=n){
            ch=response[++i];
            while(ch!='<'&&i!=n){
                printf("%c",ch);
                // ch=line[++idx];
                ch=response[++i];
            }
            while(ch!='>'&& i!=n){
                // ch=line[++idx];
                ch=response[++i];
            }
        }
        // printf("%s", response);
    }

    if (n < 0) {
        perror("read");
        return 1;
    }

    // Close the socket
    close(sockfd);

    return 0;
}
