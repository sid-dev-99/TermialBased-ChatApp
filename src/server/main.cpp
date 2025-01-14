// Server.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
using namespace std;

#define PORT 8080

class Server {
private:
    int server_fd, new_socket;
    struct sockaddr_in address; //holds ip and ports 
    int addr_len;
    char buffer[1024];

public:
    Server() {
        server_fd = 0;
        new_socket = 0;
        addr_len = sizeof(address);
        memset(buffer, 0, sizeof(buffer));
    }

    void initialize() {
        // Create socket
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE); 
        }

        // Set server address
        address.sin_family = AF_INET; //ipv4 protocoal
        address.sin_addr.s_addr = INADDR_ANY; //accept connection from any addr
        address.sin_port = htons(PORT);

        // Bind socket
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Server listening on port " << PORT << "...\n";
    }

    void acceptConnection() {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Client connected\n";
    }

    void communicate() {
        while (true) {
            memset(buffer, 0, sizeof(buffer));

            int valread = read(new_socket, buffer, sizeof(buffer));
            if (valread <= 0) {
                std::cout << "Connection closed by client\n";
                break;
            }
            std::cout << "Client: " << buffer << std::endl;

            std::cout << "Server: ";
            std::string msg;
            std::getline(std::cin, msg);
            send(new_socket, msg.c_str(), msg.length(), 0);
        }
    }


    void closeConnection() {
        close(new_socket);
        close(server_fd);
    }
};

int main() {
    Server server;
    server.initialize();
    server.acceptConnection();
    server.communicate();
    server.closeConnection();
    return 0;
}
