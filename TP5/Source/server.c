#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

// defining values for the server to answer
#define TOO_LOW 1
#define TOO_HIGH 2
#define WIN 3
#define LOSE 4
#define MAX_TRIES 15

unsigned char random_number(unsigned char min, unsigned char max) {
        unsigned char data;
        int fd = open("/dev/urandom", O_RDONLY);
        do {
            read(fd, &data, sizeof(data));
        } while (data < min || data > max);
        close(fd);
        return data;
}

int main(int argc, char* argv[]) {
    struct sockaddr_in address; // Server address struct

    // min and max value for the client to find
    unsigned char min = 0;
    unsigned char max = 150;

    // making sure that the port is a int beetwen 1024 and 65535
    //int port = 23456;
    int port = strtol(argv[1], NULL, 10);
    if (port < 1024 || port > 65535) {
        fprintf(stderr, "Port wasn't in the defined range (1024 - 65535)\n");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address)); // initialize address to 0.0.0.0
    address.sin_family = AF_INET; // Using INADDR_ANY for linking to every network interface of the computer
    address.sin_addr.s_addr = htonl(INADDR_ANY); // valid address
    address.sin_port = htons(port); // valid port

    // Creating the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket == -1) {// checking for socket creation
        fprintf(stderr, "Couldn't create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // binding socket to address
    int b_status = bind(serverSocket, (struct sockaddr*) &address, sizeof(address));
    if (b_status == -1) { // checking the bind
        fprintf(stderr, "Couldn't bind socket to address: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Printing port
    printf("Server %s Listening on port: %d\n", inet_ntoa(address.sin_addr), port);

    // Listening to a maximum of 5 interfaces
    int queue = 4;
    int l_status = listen(serverSocket, queue); // Passive mode socket
    if (l_status == -1) { // Checking if there's any problem
        fprintf(stderr, "Couldn't listen for clients: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Init client address
    accept:printf("> ");
    struct sockaddr_in clientAddress; //Client address struct
    unsigned int clientLength = sizeof(clientAddress); // Getting the length of a client address
    // Accepting connection
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLength);

    printf("Client %d with address %s:%d is connected\n", clientSocket, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

    // Creating new process for every new connection
    pid_t pid = fork();
    if (pid > 0) { // Parents PID
        // Parent's code
        close(clientSocket);
        goto accept; // Accepting new connection
    } else if (pid == 0) { // Child's PID
        // Child's code
        // We don't want any zombie process so we make a new process
        pid_t pidS = fork();
        if (pidS == 0) {
            // First communication
            // Writing min and max
            write(clientSocket, &min, sizeof(min));
            write(clientSocket, &max, sizeof(max));

            unsigned char random_value = random_number(min, max);
            printf("Random value generated for client %d: %u\n", clientSocket, random_value);

        unsigned char tries = 0;
            unsigned char left = MAX_TRIES;
            unsigned char cmd;
            for (;;) { // Checking for clients guesses
                unsigned char input;
                read(clientSocket, NULL, 1);
                read(clientSocket, &input, sizeof(input));
                printf("Client %d guessed %u\n", clientSocket, input);

                // If client makes too much tries
                if (tries >= MAX_TRIES) {
                    cmd = LOSE; // LOSE command
                    // Send info to client
                    write(clientSocket, &cmd, sizeof(cmd)); // Command
                    write(clientSocket, &random_value, sizeof(random_value)); // Right Value
                    write(clientSocket, &left, sizeof(left)); // Left tries
                    printf("Client %d lost\n", clientSocket);
                    exit(EXIT_SUCCESS);
                }
                // If client guesses right
                if (input == random_value) {
                    cmd = WIN;
                    // Sending info to client
                    write(clientSocket, &cmd, sizeof(cmd));
                    write(clientSocket, &random_value, sizeof(random_value));
                    write(clientSocket, &left, sizeof(left));
                    // Client wins
                    printf("Client %d wins\n", clientSocket);
                    exit(EXIT_SUCCESS);
                } else {
                    tries++; // Incrementing tries
                    left = MAX_TRIES - tries;
                    if (input < random_value) {
                        cmd = TOO_LOW;
                        write(clientSocket, &cmd, sizeof(cmd));
                        write(clientSocket, &random_value, sizeof(random_value));
                        write(clientSocket, &left, sizeof(left));
                    } else {
                        cmd = TOO_HIGH;
                        write(clientSocket, &cmd, sizeof(cmd));
                        write(clientSocket, &random_value, sizeof(random_value));
                        write(clientSocket, &left, sizeof(left));
                    }
                }
            }
        }
        waitpid(-1, NULL, 0); // Kill child :)
    } else {
        fprintf(stderr, "Couldn't create process: %s", strerror(errno));
    }
    return 0;
}