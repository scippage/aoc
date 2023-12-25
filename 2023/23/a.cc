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
#include <memory>
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

typedef std::vector<std::string> Trail;
typedef std::pair<int, int> Coord;
typedef std::tuple<int, Coord, Coord> QueueElm;
typedef std::priority_queue<QueueElm, std::vector<QueueElm>, std::greater<QueueElm>> Queue;

std::vector<Coord> neighbors(Trail& t, Coord& here, Coord& from) {
    // we notice that in our input, intersections are always surrounded by slopes.
    // these slopes also make cycles impossible.
    // so as long as we remove the edge going backwards there are no negative cycles.
    auto [i, j] = here;
    std::vector<Coord> res;
    std::vector<Coord> candidates;
    if (t[i][j] == '>' || t[i][j] == '.') candidates.push_back(std::make_pair(i, j+1));
    if (t[i][j] == '<' || t[i][j] == '.') candidates.push_back(std::make_pair(i, j-1));
    if (t[i][j] == '^' || t[i][j] == '.') candidates.push_back(std::make_pair(i-1, j));
    if (t[i][j] == 'v' || t[i][j] == '.') candidates.push_back(std::make_pair(i+1, j));
    for (auto candidate: candidates) {
        auto [c_i, c_j] = candidate;
        if (c_i >= 0 && c_i < t.size() && c_j >= 0 && c_j < t[0].size()) {
            if (t[c_i][c_j] != '#' && candidate != from) {
                res.push_back(candidate);
            }
        }
    }
    return res;
}

void dijkstras(Trail& t, std::map<Coord, int>& costs, Queue& queue) {
    // this is technically SPFA.
    // compared to Dijkstra's, we allow nodes to be revisited but only push them onto
    // the queue when there's potential for improvement.
    // we could've also run Bellman-Ford.
    while (!queue.empty()) {
        auto [priority, coord, from] = queue.top();
        queue.pop();
        // printf("(%d, %d): %d\n", coord.first, coord.second, costs[coord]);

        int w = -1;
        for (auto& neighbor: neighbors(t, coord, from)) {
            if (!costs.contains(neighbor)) costs[neighbor] = INT_MAX;
            if (costs[coord] + w < costs[neighbor]) {
                costs[neighbor] = costs[coord] + w;
                queue.push(std::make_tuple(costs[neighbor], neighbor, coord));
            }
        }
    }
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    Trail t;
    Coord start = std::make_pair(0, 1);
    while (std::getline(file, line)) {
        t.push_back(line);
    }

    std::map<Coord, int> costs;
    Queue queue;
    queue.push(std::make_tuple(0, start, start));
    dijkstras(t, costs, queue);
    ans = -costs[std::make_pair(t.size()-1, t[0].size()-2)];

    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}