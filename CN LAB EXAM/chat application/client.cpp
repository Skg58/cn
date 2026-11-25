#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <winsock2.h>
#include <ws2tcpip.h>

// Links the Winsock library automatically in MSVC (for MinGW, pass -lws2_32)
#pragma comment(lib, "ws2_32.lib")

std::mutex cout_mtx;
std::atomic<bool> running(true);

// Returns false if the connection was closed or an error occurred
bool read_line(SOCKET fd, std::string& out_line) {
    out_line.clear();
    char c;
    while (recv(fd, &c, 1, 0) > 0) {
        if (c == '\n') return true;
        if (c != '\r') out_line += c; // Strip Windows carriage return
    }
    return false;
}

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed. Make sure the server is running.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to server 127.0.0.1:8080\n";

    // 2. Background thread to receive messages
    std::thread receiver([sock]() {
        std::string msg;
        while (running) {
            if (!read_line(sock, msg) || msg == "exit") {
                running = false;
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "\nServer ended the chat. Press Enter to close.\n";
                break;
            }
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Server: " << msg << "\n";
        }
    });

    // 3. Main thread: send messages
    std::string msg;
    while (running && std::getline(std::cin, msg)) {
        if (!running) break;

        std::string out = msg + "\n";
        if (send(sock, out.c_str(), static_cast<int>(out.size()), 0) == SOCKET_ERROR) {
            break;
        }

        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "Client (you): " << msg << "\n";
        }

        if (msg == "exit") {
            running = false;
            break;
        }
    }

    // 4. Cleanup
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    if (receiver.joinable()) receiver.join();
    WSACleanup();

    return 0;
}
