#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);

    const char *msg = "Hello from UDP Client!";
    sendto(sockfd, msg, strlen(msg), 0, (sockaddr *)&servAddr, sizeof(servAddr));

    char buffer[1024];
    socklen_t len = sizeof(servAddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&servAddr, &len);
    buffer[n] = '\0';
    std::cout << "Server: " << buffer << "\n";

    close(sockfd);
    return 0;
}