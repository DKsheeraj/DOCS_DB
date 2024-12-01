#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_IP "192.168.0.4" 
#define SERVER_PORT 8080   
#ifndef NUM_CONNECTIONS
#define NUM_CONNECTIONS 10
#endif   

typedef struct {
    int thread_id;
    int num_requests;
} ThreadData;

void *send_requests(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int thread_id = data->thread_id;
    int num_requests = data->num_requests;
    char buffer[1024];
    int sockfd;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        pthread_exit(NULL);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        pthread_exit(NULL);
    }
    printf("Thread %d connected\n", thread_id);
    for (int i = 0; i < num_requests; i++) {
        int data_size = rand() % 512 + 512; // Random size between 512 and 1024 bytes
        memset(buffer, 'A', data_size);
        if (send(sockfd, buffer, data_size, 0) < 0) {
            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "Send failed at %d", i);
            perror(buffer);
            close(sockfd);
            pthread_exit(NULL);
        }
        //printf("Thread %d sent %d bytes\n", thread_id, data_size);
    }
    close(sockfd);
    //printf("Thread %d completed\n", thread_id);
    pthread_exit(NULL);
}

int main() {
        int connections = NUM_CONNECTIONS;
        printf("\nRunning test with %d requests over %d connections...\n", 1000*connections, connections);

        pthread_t threads[connections];
        ThreadData thread_data[connections];
        int requests_per_thread = 1000;

        srand(time(NULL));

        for (int i = 0; i < connections; i++) {
            thread_data[i].thread_id = i+1;
            thread_data[i].num_requests = requests_per_thread;
            if (pthread_create(&threads[i], NULL, send_requests, (void *)&thread_data[i]) != 0) {
                perror("Thread creation failed");
                exit(EXIT_FAILURE);
            }
        }
        for (int i = 0; i < connections; i++) {
            pthread_join(threads[i], NULL);
        }
        printf("Test with %d requests over %d connections completed.\n", 1000*connections, connections);

    return 0;
}
