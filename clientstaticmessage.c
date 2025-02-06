#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080  // Server port

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    // 1. Create a socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Socket creation failed.\n";
        return -1;
    }

    // 2. Define the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // 3. Connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection to server failed.\n";
        return -1;
    }

    std::cout << "Connected to the server!\n";

    // 4. Send a static (fixed) message to the server
    std::string staticMessage = "Hello Server, this is a fixed message from the client!";
    send(client_fd, staticMessage.c_str(), staticMessage.length(), 0);
    std::cout << "Client: " << staticMessage << std::endl;

    // 5. Receive a response from the server
    int bytesReceived = recv(client_fd, buffer, 1024, 0);
    if (bytesReceived <= 0) {
        std::cout << "Server disconnected.\n";
    } else {
        std::cout << "Server: " << buffer << std::endl;
    }

    // 6. Close the socket
    close(client_fd);
    return 0;
}
