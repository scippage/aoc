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


int64_t count_ways(std::string springs, int damaged, int changeables, std::deque<int> damaged_lens,
        std::map<std::pair<std::string, std::deque<int>>, int64_t> &cache) {
    // damaged: #s remaining
    // changeables: ?s remaining (unused)

    std::pair key = std::make_pair(springs, damaged_lens);
    if (cache.contains(key)) {
        return cache[key];
    }

    int64_t ways = 0;
    if (damaged_lens.empty()) {
        if (damaged==0) return 1;
        else return 0;
    }
    else if (springs.size() < damaged_lens.front()) {
        return 0;
    }

    // treat as .
    bool is_changeable = springs[0]=='?';
    if (springs[0] == '.' || springs[0] == '?') {
        ways += count_ways(springs.substr(1), damaged, changeables-is_changeable, damaged_lens, cache);
    }

    // treat as # run equal to len(damaged_lens[0])
    if (springs[0] == '#' || springs[0] == '?') {
        int run_len = damaged_lens.front();
        int damaged_ct = 0;
        int changeable_ct = 0;
        bool can_make_run = true;
        for (int i=0; i<run_len; ++i) {
            if (springs[i] == '.') can_make_run = false;
            if (springs[i] == '#') ++damaged_ct;
            if (springs[i] == '?') ++changeable_ct;
        }
        if (springs.size() > run_len) {
            if (springs[run_len] == '#') can_make_run = false;
            if (springs[run_len] == '?') ++changeable_ct;
        }
        if (can_make_run) {
            auto damaged_lens_copy = damaged_lens;
            damaged_lens_copy.pop_front();
            if (springs.size() > run_len) {
                ways += count_ways(springs.substr(run_len+1), damaged-damaged_ct, changeables-changeable_ct, damaged_lens_copy, cache);
            } else {
                ways += count_ways(springs.substr(run_len), damaged-damaged_ct, changeables-changeable_ct, damaged_lens_copy, cache);
            }
        }
    }
    cache[key] = ways;
    return ways;
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
        std::deque<int> damaged_lens;
        while (std::getline(iss, token, ',')) {
            damaged_lens.push_back(std::stoi(token));
        }
        int changeables = 0;
        int damaged = 0;
        for (auto& c: springs) {
            if (c == '#') ++damaged;
            if (c == '?') ++changeables;
        }

        // part b
        std::deque<int> damaged_lens_5x = damaged_lens;
        damaged_lens_5x.insert(damaged_lens_5x.end(), damaged_lens.begin(), damaged_lens.end());
        damaged_lens_5x.insert(damaged_lens_5x.end(), damaged_lens.begin(), damaged_lens.end());
        damaged_lens_5x.insert(damaged_lens_5x.end(), damaged_lens.begin(), damaged_lens.end());
        damaged_lens_5x.insert(damaged_lens_5x.end(), damaged_lens.begin(), damaged_lens.end());
        std::string springs_5x = springs + '?' + springs + '?' + springs + '?' + springs + '?' + springs;
        int damaged_5x = damaged*5;
        int changeables_5x = changeables*5;

        std::map<std::pair<std::string, std::deque<int>>, int64_t> cache;
        int64_t ways = count_ways(springs_5x, damaged_5x, changeables_5x, damaged_lens_5x, cache);
        // int ways = count_ways(springs, damaged, changeables, damaged_lens, cache);
        std::cout << ways << " " << cache.size() << " | ";
        ans += ways;

        // print_progress(float(i)/total_lines);
    }
    std::cout << std::endl;

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}