#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    // 1. Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Socket creation failed.\n";
        return -1;
    }

    // 2. Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // 3. Connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }

    std::cout << "Connected to the server!\n";

    while (true) {
        // Client types a message
        std::string message;
        std::cout << "Client (type message): ";
        std::getline(std::cin, message);

        // Send message to server
        send(client_fd, message.c_str(), message.length(), 0);

        // Receive response from server
        int bytesReceived = recv(client_fd, buffer, 1024, 0);
        if (bytesReceived <= 0) {
            std::cout << "Server disconnected.\n";
            close(client_fd);
            break;
        }

        std::cout << "Server: " << buffer << std::endl;
    }

    close(client_fd);
    return 0;
}
