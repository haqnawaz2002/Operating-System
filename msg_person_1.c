// This is the server side of the real-time message passing system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_MSG_LEN 1024

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_MSG_LEN] = {0};
    char msg[MAX_MSG_LEN] = {0};

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for client connection...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected. Start sending and receiving messages...\n");

    while (1) {
        // Read message from client
        valread = read(new_socket, buffer, MAX_MSG_LEN);
        printf("Message from client: %s\n", buffer);

        // Exit if "exit" is received
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Clear buffer
        memset(buffer, 0, MAX_MSG_LEN);

        // Send message to client
        printf("Enter message to send to client (or type 'exit' to quit): ");
        fgets(msg, MAX_MSG_LEN, stdin);
        send(new_socket, msg, strlen(msg), 0);

        // Exit if "exit" is typed
        if (strcmp(msg, "exit\n") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Clear buffer
        memset(msg, 0, MAX_MSG_LEN);
    }

    close(new_socket); // Close client socket
    close(server_fd);  // Close server socket

    return 0;
}
