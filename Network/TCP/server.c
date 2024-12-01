#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 1000);
    printf("Server listening on port 8080\n");
    int i=0;
    while(1) {i++;
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        printf("Client %d connected\n", i);
        if(fork() == 0){
            close(server_fd);
            while(1){
                int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                if (bytes_received < 0) {
                    perror("Receive failed");
                    break;
                }
            }
            printf("Client %d disconnected\n", i);
            close(client_fd);
            exit(0);
            //printf("Received: %d bytes\n", bytes_received);
        }
        close(client_fd);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
