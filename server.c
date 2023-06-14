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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    // Receive the file name
    char file_name[FILE_NAME_SIZE];
    bzero(file_name, FILE_NAME_SIZE);
    n = read(newsockfd, file_name, FILE_NAME_SIZE - 1);
    if (n < 0)
        error("ERROR reading file name");

    // Open the file for writing
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
        error("ERROR opening file");

    // Receive and write the file content
    bzero(buffer, BUFFER_SIZE);
    while ((n = read(newsockfd, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, sizeof(char), n, file);
        bzero(buffer, BUFFER_SIZE);
    }
    if (n < 0)
        error("ERROR reading file");

    printf("File received successfully.\n");

    // Close the file and socket
    fclose(file);
    close(newsockfd);
    close(sockfd);

    return 0;
}
