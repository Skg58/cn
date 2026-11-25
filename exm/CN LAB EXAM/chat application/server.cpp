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
    int srv = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(srv, (sockaddr*)&addr, sizeof(addr));
    listen(srv, 1);
    std::cout << "Server listening on port 8080...\n";

    sockaddr_in cli_addr{};
    socklen_t len = sizeof(cli_addr);
    int cli = accept(srv, (sockaddr*)&cli_addr, &len);
    std::cout << "Client connected: " << inet_ntoa(cli_addr.sin_addr) << "\n";

    if (fork() == 0) { 
        close(srv);
        while (true) {
            std::string msg = read_line(cli);
            if (msg.empty() || msg == "exit") {
                std::cout << "Client ended the chat.\n";
                break;
            }
            std::cout << "Client: " << msg << "\n";
        }
        close(cli);
        return 0;
    }

    // Parent: send
    std::string msg;
    while (std::getline(std::cin, msg)) {
        std::string out = msg + "\n";
        send(cli, out.c_str(), out.size(), 0);
        std::cout << "Server (you): " << msg << "\n";
        if (msg == "exit") break;
    }

    wait(nullptr);
    close(cli);
    close(srv);
    return 0;
}