#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define FILE_NAME_SIZE 256

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server IP> <port> <file name>\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char file_name[FILE_NAME_SIZE];
    FILE *file;

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_pton(AF_INET, argv[1], &(serv_addr.sin_addr)) <= 0)
        error("ERROR invalid server IP");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Send the file name
    strncpy(file_name, argv[3], FILE_NAME_SIZE - 1);
    n = write(sockfd, file_name, strlen(file_name));
    if (n < 0)
        error("ERROR writing to socket");

    // Open the file for reading
    file = fopen(file_name, "rb");
    if (file == NULL)
        error("ERROR opening file");

    // Send the file content
    bzero(buffer, BUFFER_SIZE);
    while ((n = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        if (write(sockfd, buffer, n) < 0)
            error("ERROR writing to socket");
        bzero(buffer, BUFFER_SIZE);
    }
    if (n < 0)
        error("ERROR reading file");

    printf("File sent successfully.\n");

    // Close the file and socket
    fclose(file);
    close(sockfd);

    return 0;
}
