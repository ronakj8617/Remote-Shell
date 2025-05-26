//
// Created by Ronak on 26/05/25.
//

#include "handle_signals.hpp"

std::atomic<int> global_socket = -1;

void handle_sigint(int sig) {
    std::cout << "\n[!] Caught SIGINT (Ctrl+C), shutting down...\n";
    if (global_socket != -1) {
        close(global_socket);  // Close socket
        std::cout << "[!] Socket closed.\n";
    }
    exit(0);  // Exit gracefully
}
