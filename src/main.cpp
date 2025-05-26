//
// Created by Ronak on 26/05/25.
//

#include "main.hpp"
#include "./client/client.hpp"
#include "./server/server.hpp"

int main() {
    signal(SIGPIPE, SIG_IGN); // Ignoring SIGPIPE globally

    int port = 8080;
    auto start = std::chrono::high_resolution_clock::now();

    // Start server in a background thread
    std::thread server_thread(run_server, port);

    // Let server start up
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Start client
    run_client("127.0.0.1", port);

    server_thread.join();
    auto end = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Time taken:" << time_taken;

    return 0;
}

