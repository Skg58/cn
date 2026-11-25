#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

// Links the Winsock library automatically in MSVC (for MinGW, pass -lws2_32)
#pragma comment(lib, "ws2_32.lib")

std::string read_line(SOCKET fd) {
    std::string line;
    char c;
    while (recv(fd, &c, 1, 0) > 0 && c != '\n') {
        if (c != '\r') line += c; // Handle Windows CRLF (\r\n)
    }
    return line;
}

int main() {
    // 1. Initialize Winsock (Mandatory on Windows)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET srv = socket(AF_INET, SOCK_STREAM, 0);
    char opt = 1; // Windows expects const char* for setsockopt
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(srv, (sockaddr*)&addr, sizeof(addr));
    listen(srv, 1);
    std::cout << "Server listening on port 8080...\n";

    sockaddr_in cli_addr{};
    int len = sizeof(cli_addr); // Windows uses int instead of socklen_t
    SOCKET cli = accept(srv, (sockaddr*)&cli_addr, &len);
    std::cout << "Client connected: " << inet_ntoa(cli_addr.sin_addr) << "\n";

    // 2. Use std::thread instead of fork() to receive messages
    std::thread receiver([cli]() {
        while (true) {
            std::string msg = read_line(cli);
            if (msg.empty() || msg == "exit") {
                std::cout << "Client ended the chat.\n";
                break;
            }
            std::cout << "Client: " << msg << "\n";
        }
    });

    // Main thread: send messages
    std::string msg;
    while (std::getline(std::cin, msg)) {
        std::string out = msg + "\n";
        send(cli, out.c_str(), (int)out.size(), 0);
        std::cout << "Server (you): " << msg << "\n";
        if (msg == "exit") break;
    }

    // 3. Cleanup using Windows functions
    closesocket(cli);
    closesocket(srv);
    if (receiver.joinable()) receiver.join();
    WSACleanup();

    return 0;
}

// g++ server.cpp -o server.exe -lws2_32
// g++ client.cpp -o client.exe -lws2_32
// ./server.exe
// ./client.exe
