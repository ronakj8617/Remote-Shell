//
// Created by Ronak on 26/05/25.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "./../handle_signals.hpp"

#pragma once
void run_server(int port = 8080);

std::string execute_command(const std::string &command);
