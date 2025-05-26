//
// Created by Ronak on 26/05/25.
//

#include "handle_signals.hpp"

std::atomic<int> global_server_fd = -1;
std::atomic<int> global_client_fd = -1;
std::atomic<int> global_socket = -1; // Optional

void handle_sigint(int sig) {
    std::cout << "\n[!] Caught SIGINT (Ctrl+C), shutting down...\n";

    std::unordered_set<int> closed;
    auto safe_close = [&](int fd, const std::string &name) {
        if (fd != -1 && closed.count(fd) == 0) {
            close(fd);
            std::cout << "[!] " << name << " closed.\n";
            closed.insert(fd);
        }
    };

    safe_close(global_client_fd, "Client socket");
    safe_close(global_server_fd, "Server socket");
    safe_close(global_socket, "Generic socket");

    exit(0);
}