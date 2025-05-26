//
// Created by Ronak on 26/05/25.
//

#include "server.hpp"


void run_server(int port) {
    signal(SIGINT, handle_sigint); // Register handler

    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return; }

    global_socket = server_fd;  // Register for SIGINT cleanup

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return;
    }

    std::cout << "[Server] Waiting for connections...\n";

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0) { perror("accept"); return; }

    global_socket = client_fd;  // Update for client socket

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = read(client_fd, buffer, 1024);
        if (bytes <= 0) break;

        std::string cmd(buffer);
        std::string result = execute_command(cmd);

        size_t total_sent = 0;
        while (total_sent < result.size()) {
            ssize_t sent = send(client_fd, result.c_str() + total_sent, result.size() - total_sent, 0);
            if (sent <= 0) break;
            total_sent += sent;
        }
    }

    close(client_fd);
    close(server_fd);
}

std::string execute_command(const std::string &cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "Error running command";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}
