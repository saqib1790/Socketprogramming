#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#define PORT 8080;
#define MAX_CLIENTS 2
void* handleclients(void* clientsocket){
    int client_fd = *(int*)clientsocket;
    char buffer[1024] = {0};
    while(true){
        int bytesrecieved = recv(client_fd , buffer , 1024 , 0);
        if(bytesrecieved<=0){
            std::cout<<"Client dissconnected"<<endl;
            close(client_fd);
            break;
        }
        std::cout << "Client: " << buffer << std::endl;
        std::string response;
        std::cout<<"Server(type Response)"<<endl;
        std::getline(std::cin , response);
        send(client_fd , response.c_str() , response.length() , 0);
        pthread_exit(NULL);
    }

}
int main(){
    int server_fd , client_fd;
    struct sockaddr_in server_addr , client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];
    server_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(server_fd == -1){
        std::cerr<<"Socket Creation Failed"<<endl;
        return -1;
    }
    //2.  Define Server Address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    // 3. Bind the Sock
    if(bind(server_fd , (struct sockaddr*)&server_addr , sizeof(server_addr))<0){
        std::cerr << "Binding failed.\n";
        return -1;
    }
    // 4. Listining
    if(listen(server_fd , MAX_CLIENTS)<0){
        std::cerr << "Listening failed.\n";
        return -1;
    }
    std::cout << "Server is waiting for connections on port " << PORT << "...\n";
    // 5. Accept multiple Clients Connections
    for(int i = 0;i<MAX_CLIENTS;i++){
        client_fd = accept(server_fd , (struct sockaddr*)&client_addr , &add_len);
        if(client_fd<0){
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

}