//
// Created by Ronak on 26/05/25.
//


#include <iostream>
#include<csignal>
#include <thread>
#include <chrono>
#include "colors.hpp"


std::unordered_map<std::string, std::string> clients;
std::unordered_map<std::string, std::string> client_containers;
std::unordered_map<std::string, std::condition_variable> conds;
std::mutex mtx;

std::string exec_in_container(const std::string &client_id, const std::string &commands);
