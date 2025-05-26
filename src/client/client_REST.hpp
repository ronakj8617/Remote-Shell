//
// Created by Ronak on 26/05/25.
//

#include<iostream>
#include<thread>
#include<crow.h>
#include<set>
#include <unordered_set>
#include<unordered_map>
#include<string>

std::unordered_map<std::string, std::string> clients;
std::unordered_map<std::string, std::condition_variable> conds;
std::mutex mtx;

void getClients();