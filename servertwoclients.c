


// Code for Server Architecture

#include <iostream>       // For input/output operations (std::cout, std::cin)
#include <cstring>        // For memory functions like memset()
#include <sys/socket.h>   // For socket programming functions (socket(), bind(), listen(), accept())
#include <netinet/in.h>   // For struct sockaddr_in (Internet address structure)
#include <unistd.h>       // For system calls like close()
#include <pthread.h>      // For multithreading (pthread_create(), pthread_join())

#define PORT 8080        // Defines the port number on which the server listens for connections
#define MAX_CLIENTS 2    // Maximum number of clients that can connect at a time

void* handleClient(void* clientSocket) {
    int client_fd = *(int*)clientSocket;  // Dereference void pointer to get client socket file descriptor
    char buffer[1024] = {0};  // Buffer to store messages received from the client

    while (true) {
        // Receive message from client
        int bytesReceived = recv(client_fd, buffer, 1024, 0);
        if (bytesReceived <= 0) {  // If recv() returns <= 0, the client has disconnected
            std::cout << "Client disconnected.\n";
            close(client_fd);
            break;
        }

        std::cout << "Client: " << buffer << std::endl;

        // Server types a response
        std::string response;
        std::cout << "Server (type response): ";
        std::getline(std::cin, response);

        // Send response to client
        send(client_fd, response.c_str(), response.length(), 0);
    }
    pthread_exit(NULL);  // End thread execution
}
int main() {
    int server_fd, client_fd;  // File descriptors for server socket and client socket
    struct sockaddr_in server_addr, client_addr;  // Structures for storing server and client addresses
    socklen_t addr_len = sizeof(client_addr);  // Variable to store address length
    pthread_t threads[MAX_CLIENTS];  // Array to store thread IDs for clients
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
     std::cerr << "Socket creation failed.\n";
     return -1;
    }
    server_addr.sin_family = AF_INET;  // Use IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any available network interface
    server_addr.sin_port = htons(PORT);  // Convert port number to network byte order
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Binding failed.\n";
    return -1;
}
if (listen(server_fd, MAX_CLIENTS) < 0) {
    std::cerr << "Listening failed.\n";
    return -1;
}
std::cout << "Server is waiting for connections on port " << PORT << "...\n";
for (int i = 0; i < MAX_CLIENTS; i++) {
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        std::cerr << "Accepting connection failed.\n";
        return -1;
    }

    std::cout << "Client connected.\n";
    pthread_create(&threads[i], NULL, handleClient, (void*)&client_fd);
}
for (int i = 0; i < MAX_CLIENTS; i++) {
    pthread_join(threads[i], NULL);
}
close(server_fd);
return 0;
