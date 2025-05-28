//
// Created by Ronak on 26/05/25.
//

#include "client.hpp"

#include "colors.hpp"

void run_client(const std::string &host, int port) {
    signal(SIGINT, handle_sigint); // Register handler

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    global_socket = sock; // Register socket for cleanup

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return;
    }

    std::cout << "[Client] Connected to server.\n";

    std::string input;
    char buffer[4096];

    // while (true)
    {
        std::cout << RED << "remote-shell> " << BLUE;
        // std::getline(std::cin, input);
        // if (input == "exit" || std::cin.eof()) break;
        input = "ls\n";

        send(sock, input.c_str(), input.length(), 0);

        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = read(sock, buffer, sizeof(buffer));
        if (bytes <= 0) {
            close(sock);
            return;
        }
        std::cout << buffer;
    }

    close(sock);
}
