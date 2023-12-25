#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <climits>
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
#include <queue>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>


enum Direction { U, D, L, R };
std::map<std::string, Direction> direction_map;
#define ADDITEM(x) direction_map[#x] = x;
void setup() {
    ADDITEM(U);
    ADDITEM(D);
    ADDITEM(L);
    ADDITEM(R);
}

std::pair<int, int> direction_delta(Direction d) {
    // di, dj
    switch (d) {
        case (U): return std::make_pair(-1, 0);
        case (D): return std::make_pair(1, 0);
        case (L): return std::make_pair(0, -1);
        case (R): return std::make_pair(0, 1);
    }
}

typedef int length;
typedef std::string color;
typedef std::tuple<Direction, length, color> instruction;

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    setup();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::vector<instruction> instructions;
    while (std::getline(file, line)) {
        Direction d = direction_map[line.substr(0, 1)];
        length l = std::stoi(line.substr(2, 2));
        color c = line.substr(4);
        instruction ins = std::make_tuple(d, l, c);
        instructions.push_back(ins);
    }

    int i = 0, j = 0;
    int area = 0;
    int boundary_pts = 0;
    for (auto& [d, l, c]: instructions) {
        // shoelace formula
        auto [di, dj] = direction_delta(d);
        int new_i = i + di*l;
        int new_j = j + dj*l;
        area += i*new_j - j*new_i;
        boundary_pts += l;
        i = new_i;
        j = new_j;
    }
    area *= 0.5;
    area = abs(area);

    // Pick's: A = i + b/2 - 1
    int interior_pts = area + 1 - (boundary_pts/2);
    ans = boundary_pts + interior_pts;
    
    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}