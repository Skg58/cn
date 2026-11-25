#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::string read_line(int fd) {
    std::string line;
    char c;
    while (recv(fd, &c, 1, 0) > 0 && c != '\n') line += c;
    return line;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (sockaddr*)&addr, sizeof(addr));
    std::cout << "Connected to server 127.0.0.1:8080\n";

    if (fork() == 0) { // Child: receive
        while (true) {
            std::string msg = read_line(sock);
            if (msg.empty() || msg == "exit") {
                std::cout << "Server ended the chat.\n";
                break;
            }
            std::cout << "Server: " << msg << "\n";
        }
        close(sock);
        return 0;
    }

    // Parent: send
    std::string msg;
    while (std::getline(std::cin, msg)) {
        std::string out = msg + "\n";
        send(sock, out.c_str(), out.size(), 0);
        std::cout << "Client (you): " << msg << "\n";
        if (msg == "exit") break;
    }

    wait(nullptr);
    close(sock);
    return 0;
}