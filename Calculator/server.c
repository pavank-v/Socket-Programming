#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Port Number not provided\n");
        exit(1);
    }

    int sockfd, newsocket, n, port_no;
    int buf_size = 1024;
    char buffer[buf_size];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Socket Creation Failed");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    port_no = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no);

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Binding Failed");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    if ((newsocket = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen)) < 0) {
        error("Error on Accept");
    }

    int num1, num2, ans, choice;

    while (1) {

        // Show the options and get the client's choice
        char *prompt = "Enter your choice: \n1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n5. Exit\n";
        n = write(newsocket, prompt, strlen(prompt));
        if (n < 0) error("Error writing to socket");
        read(newsocket, &choice, sizeof(int));
        if (choice > 5 || choice < 0){
            printf("Enter Valid Choice");
            continue;
        }
        printf("Client choice is: %d\n", choice);

        // Get the first number
        n = write(newsocket, "Enter Number 1: ", strlen("Enter Number 1: "));
        if (n < 0) error("Error writing to socket");
        read(newsocket, &num1, sizeof(int));
        printf("Client-Number 1 is: %d\n", num1);

        // Get the second number
        n = write(newsocket, "Enter Number 2: ", strlen("Enter Number 2: "));
        if (n < 0) error("Error writing to socket");
        read(newsocket, &num2, sizeof(int));
        printf("Client-Number 2 is: %d\n", num2);

        // Perform the operation based on the client's choice
        switch (choice) {
            case 1:
                ans = num1 + num2;
                break;
            case 2:
                ans = num1 - num2;
                break;
            case 3:
                ans = num1 * num2;
                break;
            case 4:
                if (num2 == 0) {
                    n = write(newsocket, "Error: Division by zero\n", strlen("Error: Division by zero\n"));
                    continue;  // Ask for another operation without exiting
                } else {
                    ans = num1 / num2;
                }
                break;
            case 5:
                close(newsocket);
                close(sockfd);
                return 0;
        }

        // Send the result back to the client
        write(newsocket, &ans, sizeof(int));
    }

    close(newsocket);
    close(sockfd);
    return 0;
}
