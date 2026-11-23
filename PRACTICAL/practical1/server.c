#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    listen(server_fd, 5);
    printf("Server waiting for client...\n");

    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }

    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int n = read(client_fd, buffer, sizeof(buffer) - 1);

        if (n <= 0)
            break;

        printf("Client: %s\n", buffer);

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        printf("Enter reply: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        write(client_fd, buffer, strlen(buffer));

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
