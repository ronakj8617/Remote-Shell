//
// Created by Ronak on 26/05/25.
//

#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include "./../handle_signals.hpp"
#pragma once

std::pair<int, std::string> run_client(std::string clientId, std::string commands, const std::string& host = "127.0.0.1", int port = 8080);
