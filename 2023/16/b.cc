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


enum Direction { left, right, up, down };
typedef std::pair<int, int> coords;
typedef std::pair<coords, Direction> beam;
typedef std::vector<std::string> grid;


void draw_visited(grid g, std::set<coords>& lit_coords) {
    for (int i=0; i<g.size(); ++i)
        for (int j=0; j<g[0].size(); ++j)
            g[i][j] = '.';
    for (auto& coord: lit_coords) {
        g[coord.first][coord.second] = '#';
    }
    for (int i=0; i<g.size(); ++i)
        std::cout << g[i] << std::endl;
}


std::optional<beam> make_beam(grid& g, coords curr, Direction dir) {
    // take a step in the desired direction
    if (dir == right && curr.second+1 < g[0].size())  {
        return std::make_pair(std::make_pair(curr.first, curr.second+1), dir);
    }
    if (dir == left && curr.second-1 >= 0)  {
        return std::make_pair(std::make_pair(curr.first, curr.second-1), dir);
    }
    if (dir == up && curr.first-1 >= 0)  {
        return std::make_pair(std::make_pair(curr.first-1, curr.second), dir);
    }
    if (dir == down && curr.first+1 < g.size())  {
        return std::make_pair(std::make_pair(curr.first+1, curr.second), dir);
    }
    return std::nullopt;
}


void simulate(grid& g, std::set<beam>& visited, beam& vec) {
    if (visited.contains(vec)) {
        return;
    }
    coords ij = vec.first;
    // printf("Visiting (%d, %d)\n", ij.first, ij.second);
    visited.insert(vec);

    if (g[ij.first][ij.second] == '.') {
        auto new_beam = make_beam(g, ij, vec.second);
        if (new_beam.has_value()) simulate(g, visited, new_beam.value());
    }
    if (g[ij.first][ij.second] == '-') {
        if (vec.second == left || vec.second == right) {
            auto new_beam = make_beam(g, ij, vec.second);
            if (new_beam.has_value()) simulate(g, visited, new_beam.value());
        } else {
            auto left_beam = make_beam(g, ij, left);
            auto right_beam = make_beam(g, ij, right);
            if (left_beam.has_value()) simulate(g, visited, left_beam.value());
            if (right_beam.has_value()) simulate(g, visited, right_beam.value());
        }
    }
    if (g[ij.first][ij.second] == '|') {
        if (vec.second == up || vec.second == down) {
            auto new_beam = make_beam(g, ij, vec.second);
            if (new_beam.has_value()) simulate(g, visited, new_beam.value());
        } else {
            auto up_beam = make_beam(g, ij, up);
            auto down_beam = make_beam(g, ij, down);
            if (up_beam.has_value()) simulate(g, visited, up_beam.value());
            if (down_beam.has_value()) simulate(g, visited, down_beam.value());
        }
    }
    if (g[ij.first][ij.second] == '/') {
        switch (vec.second) {
            case right: {
                auto up_beam = make_beam(g, ij, up);
                if (up_beam.has_value()) simulate(g, visited, up_beam.value());
                break;
            }
            case down: {
                auto left_beam = make_beam(g, ij, left);
                if (left_beam.has_value()) simulate(g, visited, left_beam.value());
                break;
            }
            case up: {
                auto right_beam = make_beam(g, ij, right);
                if (right_beam.has_value()) simulate(g, visited, right_beam.value());
                break;
            }
            case left: {
                auto down_beam = make_beam(g, ij, down);
                if (down_beam.has_value()) simulate(g, visited, down_beam.value());
                break;
            }
        }
    }
    if (g[ij.first][ij.second] == '\\') {
        switch (vec.second) {
            case right: {
                auto down_beam = make_beam(g, ij, down);
                if (down_beam.has_value()) simulate(g, visited, down_beam.value());
                break;
            }
            case down: {
                auto right_beam = make_beam(g, ij, right);
                if (right_beam.has_value()) simulate(g, visited, right_beam.value());
                break;
            }
            case up: {
                auto left_beam = make_beam(g, ij, left);
                if (left_beam.has_value()) simulate(g, visited, left_beam.value());
                break;
            }
            case left: {
                auto up_beam = make_beam(g, ij, up);
                if (up_beam.has_value()) simulate(g, visited, up_beam.value());
                break;
            }
        }
    }
}


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    grid g;
    while (std::getline(file, line)) {
        g.push_back(line);
    }
    for (int i=0; i<g.size(); ++i) {
        for (int j=0; j<g[0].size(); ++j) {
            if (i !=0 && i != g.size()-1 && j !=0 && j != g[0].size()-1) continue;
            std::vector<Direction> dirs;
            if (i == 0) dirs.push_back(Direction::down);
            if (i == g.size()-1) dirs.push_back(Direction::up);
            if (j == 0) dirs.push_back(Direction::right);
            if (j == g[0].size()-1) dirs.push_back(Direction::left);

            for (auto& dir: dirs) {
                std::set<beam> visited;
                auto start = std::make_pair(std::make_pair(i, j), dir);
                simulate(g, visited, start);
                
                std::set<coords> lit_coords;
                for (auto& beam: visited) {
                    lit_coords.insert(beam.first);
                }
                // draw_visited(g, lit_coords);
                ans = std::max(ans, (int64_t)lit_coords.size());
            }
        }
    }
    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}