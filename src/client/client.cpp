//
// Created by Ronak on 26/05/25.
//

#include "client.hpp"

#include "colors.hpp"

std::pair<int, std::string> run_client(std::string clientId, std::string commands, const std::string &host, int port) {
    signal(SIGINT, handle_sigint); // Register handler

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return {-1, "Error creating socket"};
    }

    global_socket = sock; // Register socket for cleanup

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return {-2, "Connection failed"};
    }

    std::cout << "[Client] Connected to server.\n";

    std::string input;
    char buffer[4096];

    std::cout << RED << "remote-shell> " << BLUE;
    // std::getline(std::cin, input);
    // if (input == "exit" || std::cin.eof()) break;
    input = "ls\n";

    send(sock, input.c_str(), input.length(), 0);

    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes = read(sock, buffer, sizeof(buffer));
    if (bytes <= 0) {
        close(sock);
        return {-3, "Error reading from socket"};

        // std::cout << buffer;
    }

    std::string ret(buffer);
    // Trim trailing newline (for cleaner output)
    if (!ret.empty() && ret.back() == '\n') {
        ret.pop_back();
    }
    return {0, ret};
}
