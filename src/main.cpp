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

using json = nlohmann::json;

int main() {
    auto start = std::chrono::high_resolution_clock::now();

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
    int shellPort = 8080;

    // Start server in a background thread
    // Start client
    // run_client("127.0.0.1", shellPort);

    // Let server start up
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // server_thread.join();

    CROW_ROUTE(app, "/ping").methods("GET"_method, "OPTIONS"_method)([&](const crow::request &req) {
        std::cout << GREEN << "\nPing successful\n";
        if (req.method == "OPTIONS"_method) { return allow_cors(); }
        return crow::response(200, nlohmann::json({{"message", "Ping successful"}}).dump());
    });

    CROW_ROUTE(app, "/registerClient").methods("POST"_method, "OPTIONS"_method)([&](const crow::request &req) {
        if (req.method == "OPTIONS"_method) return allow_cors();

        auto body = nlohmann::json::parse(req.body);
        std::string id = body["id"];
        std::string password = body["password"];

        if (clients.find(id) != clients.end())
            return crow::response(400, json({{"message", "Id already exists"}}).dump());

        clients[id] = password;
        std::cout << CYAN << clients.count(id);
        return crow::response(200, json({{"message", "Client registered"}}).dump());
    });

    // work in progress
    CROW_ROUTE(app, "/sendCommand").methods("POST"_method)([&](const crow::request &req) {
        std::string id = json::parse(req.body)["id"];
        if (clients.find(id) == clients.end())
            return crow::response(
                404, json({{"message", "User is not registered."}}).dump());

        close(global_server_fd);
        close(global_client_fd);
        std::thread server_thread(run_server, shellPort);
        run_client();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        server_thread.join();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string command = json::parse(req.body)["command"];
        run_client("127.0.0.1", shellPort);

        return crow::response(
            404, json({{"message", "User is not registered."}}).dump());
    });


    std::thread restThread([&app, restPort] {
        app.port(restPort).multithreaded().run();
    });

    restThread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << CYAN << "Time taken:" << time_taken;
}
