#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    printf("Connected to server!\n");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        write(sock, buffer, strlen(buffer));

        if (strcmp(buffer, "exit") == 0)
            break;

        memset(buffer, 0, sizeof(buffer));

        int n = read(sock, buffer, sizeof(buffer) - 1);

        if (n <= 0)
            break;

        printf("Server: %s\n", buffer);
    }

    close(sock);

    return 0;
}
