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

