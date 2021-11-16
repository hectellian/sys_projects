#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define TOO_LOW 1
#define TOO_HIGH 2
#define WIN 3
#define LOSE 4

int main(int argc, char* argv[]) {
    struct sockaddr_in address; // Client address struct

    char *ip = argv[1];
    int port = strtol(argv[2], NULL, 10);
    //int port = 23456;

    //Valid ip address
    memset(&address, 0, sizeof(address)); // Init the address to 0.0.0.0
    int ipAddress = inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr));
    if (ipAddress <= 0) { // 0 if ip is invalid, -1 if family is invalid
        fprintf(stderr, "Couldn't use IP address: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // Creating the Client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        fprintf(stderr, "Couldn't create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Connecting to Server
    int result = connect(clientSocket, (struct sockaddr *) &address, sizeof(address));
    if (result == -1) {
        fprintf(stderr, "Couldn't connect to server: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else {
        printf("> Connection to server's port %d successfull\n", port);
    }

    // First communication
    // Reading initial values min and max
    unsigned char min, max;
    read(clientSocket, &min, sizeof(min));
    read(clientSocket, &max, sizeof(max));
    printf("Guess range is between %u and %u\n", min, max);

    // Loop until answer is found
    for (;;) {
        int i;
        scanf("%d", &i); // Getting client input
        unsigned char input = (unsigned char) i;
        //printf("%d\n", input);
        // Writing input
        write(clientSocket, NULL, 1);
        write(clientSocket, &input, sizeof(input));

        // Getting info from server
        unsigned char cmd, value, left;
        read(clientSocket, &cmd, sizeof(cmd));
        read(clientSocket, &value, sizeof(value));
        read(clientSocket, &left, sizeof(left));

        //printf("%d\n", cmd);

        // Checking values returned from server
        if (cmd == WIN) {
            printf("Congratulation, you won with %d tried left\n", left);
                close(clientSocket);
                break;
        } else {
            switch(cmd) {
            case LOSE:
                printf("Sorry, you lost, Answer was %u. Try again\n", value);
                close(clientSocket);
                exit(EXIT_SUCCESS);
            case TOO_LOW:
                printf("Tries left: %u\n", left);
                printf("Guess %u is too low (should be %u but shhhh)\n", input, value);
                break;
            case TOO_HIGH:
                printf("Tries left: %u\n", left);
                printf("Guess %u is too high (should be %u but shhhh)\n", input, value);
                break;
            }
        }
    }   
    return 0;
}