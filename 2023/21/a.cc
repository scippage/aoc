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
#include "../util/util.h"


typedef std::string Row;
typedef std::vector<Row> Garden;
typedef std::pair<int, int> Coord;
typedef std::pair<int, Coord> QueueElm;
typedef std::priority_queue<QueueElm, std::vector<QueueElm>, std::greater<QueueElm>> Queue;

std::vector<Coord> neighbors(Garden& g, Coord& here) {
    std::vector<Coord> res;
    auto [i, j] = here;
    Coord candidates[] = {std::make_pair(i+1, j), std::make_pair(i-1, j), std::make_pair(i, j+1), std::make_pair(i, j-1)};
    for (auto candidate: candidates) {
        auto [c_i, c_j] = candidate;
        if (c_i >= 0 && c_i < g.size() && c_j >= 0 && c_j < g[0].size()) {
            if (g[c_i][c_j] == '.') {
                res.push_back(candidate);
            }
        }
    }
    return res;
}

void dijkstras(Garden& g, std::set<Coord>& visited, std::map<Coord, int>& cost, Queue& queue) {
    while (!queue.empty()) {
        auto [priority, coord] = queue.top();
        queue.pop();
        if (visited.contains(coord)) continue;
        visited.insert(coord);
        // printf("(%d, %d): %d\n", coord.first, coord.second, cost[coord]);

        for (auto& neighbor: neighbors(g, coord)) {
            if (!cost.contains(neighbor)) cost[neighbor] = INT_MAX;
            cost[neighbor] = std::min(cost[neighbor], cost[coord] + 1);
            queue.push(std::make_pair(cost[neighbor], neighbor));
        }
    }
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    Garden g;
    while (std::getline(file, line)) {
        g.push_back(line);
    }

    Coord start_coord;
    for (int i=0; i<g.size(); ++i) {
        for (int j=0; j<g[0].size(); ++j) {
            if (g[i][j] == 'S') {
                start_coord = std::make_pair(i, j);
            }
        }
    }

    std::set<Coord> visited;
    Queue queue;
    std::map<Coord, int> cost;
    queue.push(std::make_pair(0, start_coord));
    cost[start_coord] = 0;
    dijkstras(g, visited, cost, queue);
    
    for (int i=0; i<g.size(); ++i) {
        for (int j=0; j<g[0].size(); ++j) {
            Coord c = std::make_pair(i, j);
            if (!cost.contains(c)) continue;
            // printf("(%d, %d): %d\n", i, j, cost[c]);
            if (cost[c] <= 64 && cost[c] % 2 == 0) {
                ++ans;
            }
        }
    }

    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}