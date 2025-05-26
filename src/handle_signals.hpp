//
// Created by Ronak on 26/05/25.
//

#include <iostream>
#include <unistd.h>
#include<csignal>
#include <atomic>

extern std::atomic<int> global_socket;

void handle_sigint(int sig);