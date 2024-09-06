#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    if (argc < 3){
        fprintf(stderr, "Please Provide Host ID and Port Number");
        exit(1);
    }
    int sockfd, port_no, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int buff_size = 1024;
    char buffer[buff_size];
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("Socket Creation Failed");
    }
    port_no = atoi(argv[2]);

    server = gethostbyname(argv[1]);
    if (server == NULL){
        fprintf(stderr, "ERROR, No such Host");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
        error("Connection Failed");
    }
    int num1, num2, ans, choice;
    while (1){
        bzero(buffer, buff_size);
        n = read(sockfd, buffer, buff_size);
        if (n < 0){
            error("Error reading from socket");
        }
        printf("Server : %s\n", buffer);
        scanf("%d", &num1);
        write(sockfd, &num1, sizeof(int));

        bzero(buffer, buff_size);
        n = read(sockfd, buffer, buff_size);
        if (n < 0){
            error("Error reading from socket");
        }
        printf("Server : %s\n", buffer);
        scanf("%d", &num2);
        write(sockfd, &num2, sizeof(int));

        bzero(buffer, buff_size);
        n = read(sockfd, buffer, buff_size);
        if (n < 0){
            error("Error reading from socket");
        }
        printf("Server : %s\n", buffer);
        scanf("%d", &choice);
        write(sockfd, &choice, sizeof(int));

        if (choice == 5){
            printf("You have selected to Exit");
            close(sockfd);
            return 0;
        }
        read(sockfd, &ans, sizeof(int));
        printf("Server answer is:%d\n", ans);
    }
}