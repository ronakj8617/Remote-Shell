//
// Created by Ronak on 26/05/25.
//

#include "server.hpp"

#include <thread>

#include "colors.hpp"

void run_server(int port) {
    signal(SIGINT, handle_sigint);

    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind");
        return;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        return;
    }

    global_server_fd = server_fd;
    std::cout << RED << "[Server] Listening on port " << port << "...\n";

    while (true) {
        int client_fd = accept(server_fd, (struct sockaddr *) &address, &addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        std::thread([client_fd]() {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            ssize_t bytes = read(client_fd, buffer, sizeof(buffer));
            if (bytes > 0) {
                std::string cmd(buffer);
                std::string result = execute_command(cmd);

                size_t total_sent = 0;
                while (total_sent < result.size()) {
                    ssize_t sent = send(client_fd, result.c_str() + total_sent, result.size() - total_sent, 0);
                    if (sent <= 0) break;
                    total_sent += sent;
                }
            }

            close(client_fd); // clean up after client
        }).detach(); // detached thread per client
    }

    close(server_fd);
}


std::string execute_command(const std::string &cmd) {
    std::array<char, 128> buffer;
    std::string result;
    // FILE *pipe = popen(cmd.c_str(), "r");
    FILE *pipe = popen((cmd + " 2>&1").c_str(), "r"); // Redirect stderr to stdout

    if (!pipe) return "Error running command";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}
