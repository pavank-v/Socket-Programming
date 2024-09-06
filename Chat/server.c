#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int args, char *argv[]){
    if (args < 2){
        fprintf(stderr, "Port Number is not Provided\n");
        exit(1);
    }

    int sockfd, newsocket, n, port_no;
    int buff_size = 1024;
    char buffer[buff_size];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Socket Creation Failed");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    port_no = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        error("binding failed");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsocket = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

    if (newsocket < 0){
        error("Error on Accept");
    }

    while(1){
        bzero(buffer, buff_size);
        n = read(newsocket, buffer, buff_size);
        if (n < 0){
            error("Error on Reading");
        }
        printf("Client Sent:%s", buffer);
        bzero(buffer, buff_size);
        fgets(buffer, buff_size, stdin);

        n = write(newsocket, buffer, strlen(buffer));
        if (n < 0){
            error("Error on Writing");
        }
        int i = strncmp("bye", buffer, 3);

        if (i == 0)
            break;
    }
    close(newsocket);
    close(sockfd);
    return 0;
}