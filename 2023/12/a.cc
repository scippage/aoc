#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <deque>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>


void print_progress(float progress) {
    int bar_width = 70;
    std::cout << "[";
    int pos = bar_width * progress;
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

bool validate_way(std::string springs, std::vector<int> &damaged_lens) {
    int run_len = 0;
    std::vector<int> run_lens;
    for (size_t i=0; i<springs.size(); ++i) {
        if (springs[i] == '#') ++run_len;
        if (run_len != 0 && ((springs[i] != '#') || i==springs.size()-1)) {
            run_lens.push_back(run_len);
            run_len = 0;
        }
    }
    return damaged_lens == run_lens;
}

int count_ways(std::string springs, size_t idx, int budget, int changeables, std::vector<int> &damaged_lens) {
    if (budget == 0) {
        if (validate_way(springs, damaged_lens)) return 1;
        else return 0;
    }
    if (idx >= springs.size()) return 0;
    if (budget > changeables) return 0;

    if (springs[idx] == '?') --changeables;
    int ways_keep = count_ways(springs, idx+1, budget, changeables, damaged_lens);
    int ways_change = 0;
    if (springs[idx] == '?') {
        springs[idx] = '#';
        ways_change = count_ways(springs, idx+1, budget-1, changeables, damaged_lens);
    }
    return ways_keep + ways_change;
}

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    int total_lines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
    file.clear();
    file.seekg(0);
    for (int i=0; std::getline(file, line); ++i) {    
        std::string springs = line.substr(0, line.find(' '));

        std::istringstream iss(line.substr(line.find(' ')+1));
        std::string token;
        std::vector<int> damaged_lens;
        while (std::getline(iss, token, ',')) {
            damaged_lens.push_back(std::stoi(token));
        }
        int budget = std::reduce(damaged_lens.begin(), damaged_lens.end());
        int changeables = 0;
        for (auto& c: springs) {
            if (c == '#') --budget;
            if (c == '?') ++changeables;
        }

        int ways = count_ways(springs, 0, budget, changeables, damaged_lens);
        std::cout << ways << " ";
        ans += ways;

        print_progress(float(i)/total_lines);
    }
    std::cout << std::endl;

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}