//
// Created by Ronak on 26/05/25.
//

// #define CROW_MAIN
#include "crow.h"
#include "main.hpp"
#include "./client/client.hpp"
#include "./server/server.hpp"
#include <nlohmann/json.hpp>

#include "cors.h"


int main() {
    signal(SIGPIPE, SIG_IGN); // Ignoring SIGPIPE globally

    crow::App<CORS> app;

    int restPort = 8081;
    auto allow_cors = []() {
        crow::response res;
        res.code = 204;
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    };

    CROW_ROUTE(app, "/ping").methods("GET"_method, "OPTIONS"_method)([&](const crow::request &req) {
        std::cout << GREEN << "\nPing successful\n";
        if (req.method == "OPTIONS"_method) { return allow_cors(); }
        return crow::response(200, nlohmann::json({{"message", "Ping successful"}}).dump());
    });

    std::thread restThread([&app, restPort] {
        app.port(restPort).multithreaded().run();
    });
    restThread.join();

    int shellPort = 8080;
    auto start = std::chrono::high_resolution_clock::now();

    // Start server in a background thread
    std::thread server_thread(run_server, shellPort);

    // Let server start up
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Start client
    run_client("127.0.0.1", shellPort);

    server_thread.join();
    auto end = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Time taken:" << time_taken;

}
