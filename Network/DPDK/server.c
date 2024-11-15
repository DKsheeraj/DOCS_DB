#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
#include <sys/ioctl.h>

#include <ff_config.h>
#include <ff_api.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Initialize F-Stack
    if (ff_init(argc, argv) != 0) {
        perror("F-Stack initialization failed");
        return -1;
    }

    // Start F-Stack threads
    ff_run(NULL, 0);

    int server_fd, new_socket, valread;
    struct sockaddr_in address, client_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};
    char response[32];

    // Create a socket
    if ((server_fd = ff_socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (ff_bind(server_fd, (struct linux_sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (ff_listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    socklen_t addr_len = sizeof(client_addr);

    // Accept a connection
    if ((new_socket = ff_accept(server_fd, (struct linux_sockaddr *)&client_addr, &addr_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Read data from the client
    valread = ff_read(new_socket, buffer, MAX_BUFFER_SIZE);
    printf("Received %d bytes from client: %s\n", valread, buffer);

    // Send the response (length of received message)
    snprintf(response, sizeof(response), "%d", valread);
    ff_send(new_socket, response, strlen(response), 0);
    printf("Sent response: %s\n", response);

    // Close the connection
    ff_close(new_socket);
    ff_close(server_fd);

    return 0;
}
