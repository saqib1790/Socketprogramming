#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080  // Server listens on port 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024] = {0};

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed.\n";
        return -1;
    }

    // 2. Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(PORT);

    // 3. Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Binding failed.\n";
        return -1;
    }

    // 4. Listen for a single client
    if (listen(server_fd, 1) < 0) {
        std::cerr << "Listening failed.\n";
        return -1;
    }

    std::cout << "Server is waiting for a client on port " << PORT << "...\n";

    // 5. Accept client connection
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        std::cerr << "Accepting connection failed.\n";
        return -1;
    }

    std::cout << "Client connected!\n";

    // 6. Receive message from client
    recv(client_fd, buffer, 1024, 0);
    std::cout << "Client: " << buffer << std::endl;

    // 7. Send predefined static message to client
    std::string staticResponse = "Hello Client, this is a static response from the server!";
    send(client_fd, staticResponse.c_str(), staticResponse.size(), 0);

    // 8. Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
