//
// Created by Ronak on 26/05/25.
//

#include <iostream>
#include <unistd.h>
#include<csignal>
#include<unordered_set>
#include <atomic>


extern std::atomic<int> global_socket;
extern std::atomic<int> global_client_fd;
extern std::atomic<int> global_server_fd;

void handle_sigint(int sig);
