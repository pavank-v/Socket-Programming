#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    if (argc < 2){
        fprintf(stderr, "Port Number not provided...");
        exit(1);
    }

    int sockfd, newsocket, port_no, n;
    int buff_size = 1024;
    char buffer[buff_size];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Socket Creation Failed");
    }

    port_no = atoi(argv[1]);
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        error("Bind failed");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    if ((newsocket = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen)) < 0){
        error("Socket Creation Failed");
    }
    
    FILE *f;
    int words, ch = 0;

    read(newsocket, &words, sizeof(int));
    f = fopen("file_received.txt", "a");

    while (ch != words){
        read(newsocket, buffer, buff_size);
        fprintf(f, "%s", buffer);
        ch++;
    }

    printf("The file has been successfully received.\nIt is saved in file_received.txt\n");
    close(newsocket);
    close(sockfd);
    return 0;
}