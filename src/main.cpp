//
// Created by Ronak on 26/05/25.
//

#define CROW_MAIN
#include "crow.h"
#include "main.hpp"
#include "./client/client.hpp"
#include "./server/server.hpp"
#include <nlohmann/json.hpp>

#include "cors.h"

#include <cstdio>
#include <memory>
#include <array>
#include <string>
#include <iostream>
#include <thread>
#include <csignal>
#include <chrono>
#include <unordered_map>

using json = nlohmann::json;

bool is_container_running(const std::string &name) {
    std::string cmd = "docker inspect -f '{{.State.Running}}' " + name + " 2>/dev/null";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return false;
    char buffer[16];
    std::string result;
    if (fgets(buffer, sizeof(buffer), pipe))
        result = buffer;
    pclose(pipe);
    return (result.find("true") != std::string::npos);
}

bool start_container(const std::string &name) {
    std::string cmd = "docker start " + name + " 2>&1";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return false;

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    int ret_code = pclose(pipe);

    std::cout << "[DEBUG] Start Container Output: " << result << std::endl;
    std::cout << "[DEBUG] Start Container Exit Code: " << ret_code << std::endl;

    return ret_code == 0 && is_container_running(name);
}

bool container_exists(const std::string &name) {
    std::string cmd = "docker ps -a --filter \"name=" + name + "\" --format '{{.Names}}'";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return false;
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe)) result += buffer;
    pclose(pipe);
    return (result.find(name) != std::string::npos);
}

std::string exec_in_container(const std::string &container_name, const std::string &commands) {
    std::string docker_cmd = "docker exec " + container_name + " sh -c \"" + commands + " 2>&1\"";

    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(docker_cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to run docker exec");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    int exit_code = pclose(pipe.release());

    std::cout << "[DEBUG] exec_in_container: exit_code = " << exit_code << ", output = " << result << std::endl;

    if (exit_code != 0 ||
        result.find("is not running") != std::string::npos ||
        result.find("No such container") != std::string::npos) {
        throw std::runtime_error("Container execution failed: " + result);
    }

    return result;
}

bool create_container(const std::string &name) {
    std::string cmd = "docker run -dit --name " + name + " remote-shell-image /bin/bash";

    int ret = system(cmd.c_str());
    std::cout << "[DEBUG] Create Container Exit Code: " << ret << std::endl;
    return ret == 0;
}

void cleanup_containers() {
    std::string cmd = "docker ps -a --filter \"name=client_\" --format \"{{.Names}}\"";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string container_name = buffer;
        container_name.erase(container_name.find_last_not_of(" \n\r\t") + 1); // trim
        std::string rm_cmd = "docker rm -f " + container_name;
        std::cout << "[CLEANUP] Removing container: " << container_name << std::endl;
        system(rm_cmd.c_str());
    }
    pclose(pipe);
}

void signal_handler(int signal) {
    std::cout << "\n[EXIT] Caught signal: " << signal << ". Cleaning up containers...\n";
    cleanup_containers();
    exit(0);
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    signal(SIGPIPE, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    crow::App<CORS> app;

    int restPort = 8081;
    int shellPort = 8080;

    auto allow_cors = []() {
        crow::response res;
        res.code = 204;
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    };

    std::thread server_thread(run_server, shellPort);

    CROW_ROUTE(app, "/ping").methods("GET"_method, "OPTIONS"_method)([&](const crow::request &req) {
        std::cout << "Ping successful\n";
        if (req.method == "OPTIONS"_method) { return allow_cors(); }
        return crow::response(200, json({{"message", "Ping successful"}}).dump());
    });

    CROW_ROUTE(app, "/registerClient").methods("POST"_method, "OPTIONS"_method)([&](const crow::request &req) {
        if (req.method == "OPTIONS"_method) return allow_cors();

        auto body = nlohmann::json::parse(req.body);
        std::string id = body["id"];
        std::string password = body["password"];
        std::string container_name = "client_" + id;

        if (clients.find(id) != clients.end()) {
            std::cout << "Client ID already exists in registry: " << id << "\n";
            return crow::response(400, json({{"message", "Id already exists"}}).dump());
        }

        if (container_exists(container_name)) {
            std::cout << "Container already exists: " << container_name << "\n";
            if (!is_container_running(container_name)) {
                std::cout << "Starting existing container: " << container_name << "\n";
                if (!start_container(container_name)) {
                    return crow::response(500, json({{"message", "Failed to start existing container"}}).dump());
                }
            }
        } else {
            std::cout << "Creating new container: " << container_name << "\n";
            if (!create_container(container_name)) {
                std::cerr << "Docker run failed for container: " << container_name << "\n";
                return crow::response(500, json({{"message", "Failed to create container"}}).dump());
            }
        }

        clients[id] = password;
        std::cout << "Client registered successfully: " << id << "\n";
        return crow::response(200, json({{"message", "Client registered and container ready"}}).dump());
    });

    CROW_ROUTE(app, "/sendCommand").methods("POST"_method)([](const crow::request &req) {
        auto body = json::parse(req.body);
        std::string client_id = body["client_id"];
        std::string command = body["command"];

        std::string container_name = "client_" + client_id;

        if (!container_exists(container_name)) {
            return crow::response(404, json({{"message", "Container not found"}}).dump());
        }

        if (!is_container_running(container_name)) {
            std::cout << "Container exists but not running. Trying to start: " << container_name << "\n";
            if (!start_container(container_name)) {
                std::cerr << "Failed to start container. Attempting to recreate...\n";
                system(("docker rm " + container_name).c_str());

                if (!create_container(container_name)) {
                    return crow::response(500, json({{"message", "Failed to recreate container"}}).dump());
                }
            }
        }

        try {
            std::string output = exec_in_container(container_name, command);
            return crow::response(200, json({{"output", output}}).dump());
        } catch (const std::exception &e) {
            return crow::response(500, json({{"message", e.what()}}).dump());
        }
    });

    std::thread restThread([&app, restPort] {
        app.port(restPort).multithreaded().run();
    });

    restThread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Time taken:" << time_taken << " seconds\n";
}
