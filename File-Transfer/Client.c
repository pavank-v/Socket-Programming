#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    int sockfd, port_no, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int buff_size = 1024;
    char buffer[buff_size];

    if (argc < 3){
        fprintf(stderr, "Port Number not provided\n");
        exit(1);
    }
    port_no = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        error("Error in Socket Creation");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL){
        fprintf(stderr, "ERROR, no such host");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        error("connection Failed");
    }
    FILE *f;
    int words = 0;
    char c, ch;

    f = fopen("file.txt", "r");

    while ((c = getc(f)) != EOF){
        fscanf(f, "%s", buffer);
        if (isspace(c) || c == '\t')
            words ++;
    }

    write(sockfd, &words, sizeof(int));
    rewind(f);

    while (ch != EOF){
        fscanf(f, "%s", buffer);
        write(sockfd, buffer, buff_size);
        ch = fgetc(f);
    }

    printf("File has been successfully sent\n");
    close(sockfd);
    return 0;
}