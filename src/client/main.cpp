#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>


#define PORT 8080

class Client {
private:
    int sock;
    struct sockaddr_in server_address;
    char buffer[1024];

public:
    Client() {
        sock = 0;
        memset(buffer, 0, sizeof(buffer));
    }

    void initialize(const std::string &server_ip) {
        // Create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);

        if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
            perror("Invalid address");
            exit(EXIT_FAILURE); 
        }

        // Connect to server
        if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Connected to server\n";
    }

    void communicate() {
        while (true) {
            std::cout << "Client: ";
            std::string msg;
            std::getline(std::cin, msg);

            send(sock, msg.c_str(), msg.length(), 0);

            memset(buffer, 0, sizeof(buffer));
            int valread = read(sock, buffer, sizeof(buffer));
            if (valread <= 0) {
                std::cout << "Server closed connection\n";
                break;
            }

            std::cout << "Server: " << buffer << std::endl;
        }
    }

    void closeConnection() {
        close(sock);
    }
};

int main() {
    Client client;
    client.initialize("127.0.0.1");  
    client.communicate();
    client.closeConnection();
    return 0;
}
