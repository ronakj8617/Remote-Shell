//
// Created by Ronak on 26/05/25.
//


#include <iostream>
#include<csignal>
#include <thread>
#include <chrono>
#include "colors.hpp"


std::unordered_map<std::string, std::string> clients;
std::unordered_map<std::string, std::condition_variable> conds;
std::mutex mtx;