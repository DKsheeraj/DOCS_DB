#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024

void generate_random_data(char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = 'A' + rand() % 26;
    }
}

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};
    char response[32];
    char server_ip[16];

    srand(time(NULL));

    printf("Enter server IP address: ");
    scanf("%s", server_ip);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    size_t data_size = (rand() % (MAX_BUFFER_SIZE / 2)) + (MAX_BUFFER_SIZE / 2);
    generate_random_data(buffer, data_size);

    send(sock, buffer, data_size, 0);
    printf("Sent %zu bytes to the server\n", data_size);

    valread = read(sock, response, sizeof(response));
    response[valread] = '\0';
    printf("Server response: %s\n", response);

    close(sock);

    return 0;
}
