#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::map<std::string, int> target_map = {
    {"red", 12},
    {"green", 13},
    {"blue", 14},
};

void process_ball(std::string const& ball, std::map<std::string, int>& pull_map) {
    // std::cout << ball << std::endl;
    int qty = stoi(ball.substr(0, ball.find(" ")));
    std::string color = ball.substr(ball.find(" ") + 1, std::string::npos);
    pull_map[color] = std::max(pull_map[color], qty);
}

int map_power(std::map<std::string, int>& map) {
    int res = 1;
    for (auto& map_entry : map) {
        res *= map_entry.second;
    }
    return res;
}

int process(std::string line) {
    std::cout << line << std::endl;
    
    std::string delim = ": ";
    size_t pos = line.find(": ");
    std::string pulls = line.substr(pos + delim.length(), std::string::npos);

    std::string pull;
    delim = "; ";
    std::vector<std::string> pulls_vec;
    while ((pos = pulls.find(delim)) != std::string::npos) {
        pull = pulls.substr(0, pos);
        pulls_vec.push_back(pull);
        pulls.erase(0, pos + delim.length());
    }
    pulls_vec.push_back(pulls);

    delim = ", ";
    std::map<std::string, int> pull_map = {
        {"red", 0},
        {"green", 0},
        {"blue", 0},
    };
    for (auto& pull : pulls_vec) {
        while ((pos = pull.find(delim)) != std::string::npos) {
            std::string ball = pull.substr(0, pos);
            process_ball(ball, pull_map);
            pull.erase(0, pos + delim.length());
        }
        process_ball(pull, pull_map);
    }
    return map_power(pull_map);
}

int main() {
    int ans = 0;

    std::ifstream file("./input.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            int result = process(line);
            std::cout << result << std::endl;
            ans += result;
        }
        file.close();
    }

    printf("Ans: %d\n", ans);
    exit(EXIT_SUCCESS);
}