#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in servAddr{}, cliAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(8080);

    bind(sockfd, (sockaddr *)&servAddr, sizeof(servAddr));
    std::cout << "UDP Server listening on port 8080...\n";

    char buffer[1024];
    socklen_t len = sizeof(cliAddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&cliAddr, &len);
    buffer[n] = '\0';
    std::cout << "Client: " << buffer << "\n";

    const char *reply = "Hello from UDP Server!";
    sendto(sockfd, reply, strlen(reply), 0, (sockaddr *)&cliAddr, len);

    close(sockfd);
    return 0;
}