// This is the client side of the real-time message passing system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_MSG_LEN 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_MSG_LEN] = {0};
    char msg[MAX_MSG_LEN] = {0};

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to server. Start sending and receiving messages...\n");

    while (1) {
        // Send message to server
        printf("Enter message to send (or type 'exit' to quit): ");
        fgets(msg, MAX_MSG_LEN, stdin);
        send(sock, msg, strlen(msg), 0);

        // Exit if "exit" is typed
        if (strcmp(msg, "exit\n") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Read reply from server
        valread = read(sock, buffer, MAX_MSG_LEN);
        printf("Message from server: %s\n", buffer);

        // Clear buffers
        memset(msg, 0, MAX_MSG_LEN);
        memset(buffer, 0, MAX_MSG_LEN);
    }

    close(sock); // Close socket

    return 0;
}
