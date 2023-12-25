#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
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


enum Direction { left, right, up, down };
typedef std::pair<int, int> coords;
typedef int steps;
typedef std::tuple<coords, Direction, steps> beam;
typedef std::pair<int, beam> priority_beam;
typedef std::vector<std::vector<int>> grid;
typedef std::priority_queue<priority_beam, std::vector<priority_beam>, std::greater<priority_beam>> beam_queue;

void print_costs(grid g, std::map<beam, int> costs) {
    std::map<coords, int> coord_costs;
    for (auto& [b, cost]: costs) {
        auto [coords, dir, steps] = b;
        if (!coord_costs.contains(coords)) coord_costs[coords] = INT_MAX;
        coord_costs[coords] = std::min(coord_costs[coords], cost);
    }
    for (int i=0; i<g.size(); ++i) {
        for (int j=0; j<g[0].size(); ++j) {
            int cost = coord_costs[std::make_pair(i, j)];
            std::cout << cost << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<beam> neighbors(beam& vec) {
    auto [coords, dir, steps] = vec;
    auto [i, j] = coords;
    std::vector<beam> beams;
    switch (dir) {
        case Direction::left: {
            beams.push_back(std::make_tuple(std::make_pair(i, j-1), Direction::left, steps+1));
            beams.push_back(std::make_tuple(std::make_pair(i-1, j), Direction::up, 1));
            beams.push_back(std::make_tuple(std::make_pair(i+1, j), Direction::down, 1));
            break;
        }
        case Direction::right: {
            beams.push_back(std::make_tuple(std::make_pair(i, j+1), Direction::right, steps+1));
            beams.push_back(std::make_tuple(std::make_pair(i-1, j), Direction::up, 1));
            beams.push_back(std::make_tuple(std::make_pair(i+1, j), Direction::down, 1));
            break;
        }
        case Direction::up: {
            beams.push_back(std::make_tuple(std::make_pair(i-1, j), Direction::up, steps+1));
            beams.push_back(std::make_tuple(std::make_pair(i, j-1), Direction::left, 1));
            beams.push_back(std::make_tuple(std::make_pair(i, j+1), Direction::right, 1));
            break;
        }
        case Direction::down: {
            beams.push_back(std::make_tuple(std::make_pair(i+1, j), Direction::down, steps+1));
            beams.push_back(std::make_tuple(std::make_pair(i, j-1), Direction::left, 1));
            beams.push_back(std::make_tuple(std::make_pair(i, j+1), Direction::right, 1));
            break;
        }
    }
    return beams;
}

bool valid_coords(grid& g, coords& c) {
    auto [i, j] = c;
    return (
        i >= 0 &&
        i < g.size() &&
        j >= 0 &&
        j < g[0].size()
    );
}

bool valid_beam(grid& g, beam& b) {
    auto [coords, dir, steps] = b;
    return (valid_coords(g, coords) && steps <= 3);
}

int dijkstras(grid& g, std::set<beam>& visited, std::map<beam, int>& cost, beam_queue& queue) {
    while (!queue.empty()) {
        // cost[beam] is the cost to get to (not including) beam
        auto [priority, vec] = queue.top();
        queue.pop();
        auto [coords, dir, steps] = vec;
        auto [i, j] = coords;
        if (visited.contains(vec)) continue;
        if (i == g.size()-1 && j == g[0].size()-1) return cost[vec] + g[i][j] - g[0][0];
        // printf("Visiting (%d, %d) %d\n", i, j, steps);
        visited.insert(vec);

        for (auto& neighbor: neighbors(vec)) {
            if (!valid_beam(g, neighbor)) continue;
            if (!cost.contains(neighbor)) cost[neighbor] = INT_MAX;
            cost[neighbor] = std::min(cost[neighbor], cost[vec] + g[i][j]);
            priority_beam p_neighbor = std::make_pair(cost[neighbor], neighbor);
            queue.push(p_neighbor);
        }
    }
    return -1;
}

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    grid g;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char c: line) row.push_back(c - '0');
        g.push_back(row);
    }

    std::set<beam> visited;
    std::map<beam, int> cost;
    beam_queue queue;
    priority_beam start = std::make_pair(
        0,
        std::make_tuple(std::make_pair(0, 0), Direction::right, 0)
    );
    queue.push(start);
    ans = dijkstras(g, visited, cost, queue);
    // print_costs(g, cost);
    
    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}