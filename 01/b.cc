#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <fstream>
#include <iostream>
#include <string>

std::map<std::string, int> map = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

int process(std::string line) {
    int first = -1;
    int last = -1;
    std::cout << line << std::endl;

    size_t best_pos = (size_t)-1;
    for (const auto& elem : map) {
        size_t const pos = line.find(elem.first);
        if (pos != std::string::npos && pos < best_pos) {
            best_pos = pos;
            first = elem.second;
        }
    }

    size_t worst_pos = 0;
    for (const auto& elem : map) {
        size_t const pos = line.rfind(elem.first);
        if (pos != std::string::npos && pos > worst_pos) {
            worst_pos = pos;
            last = elem.second;
        }
    }

    // std::cout << first << std::endl;
    // std::cout << last << std::endl;

    for (int i = 0; i < std::min(best_pos, line.size()); i++) {
        if (isdigit(line[i])) {
            first = (int)line[i] - '0';
            break;
        }
    }
    for (int i = line.size()-1; i >= worst_pos; i--) {
        if (isdigit(line[i])) {
            last = (int)line[i] - '0';
            break;
        }
    }
    int num = first*10 + last;
    std::cout << num << std::endl;
    return num;
}

int main() {
    int ans = 0;

    for (const auto& elem : map) {
        std::cout << elem.first << " " << elem.second << "\n";
    }

    std::ifstream file("./input.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            ans += process(line);
        }
        file.close();
    }

    printf("Ans: %d\n", ans);
    exit(EXIT_SUCCESS);
}