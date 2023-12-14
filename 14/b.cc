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


typedef std::string row;
typedef std::vector<row> grid;


int find_next_free_row(grid &g, size_t col, int starting_row) {
    for (int i=starting_row; i<g.size(); ++i) {
        if (g[i][col] == '.') {
            return i;
        }
    }
    return -1;
}


void pull_up(grid &g) {
    std::map<size_t, size_t> col_to_free_row;
    for (size_t j=0; j<g[0].size(); ++j) {
        col_to_free_row[j] = find_next_free_row(g, j, 0);
    }
    
    for (size_t j=0; j<g[0].size(); ++j) {
        for (size_t i=0; i<g.size(); ++i) {
            int free_row_idx = col_to_free_row[j];
            if (i < free_row_idx || free_row_idx == -1) continue;

            if (g[i][j] == 'O') {
                g[free_row_idx][j] = 'O';
                g[i][j] = '.';
                col_to_free_row[j] = find_next_free_row(g, j, free_row_idx+1);
            }
            if (g[i][j] == '#') {
                col_to_free_row[j] = find_next_free_row(g, j, i+1);
            }
        }
    }
}


// assuming square, even length
void rot_90_cw(grid &g) {
    size_t n = g.size();
    for (size_t i=0; i<n/2; ++i) {
        for (size_t j=0; j<n/2; ++j) {
            auto tmp = g[i][j];
            g[i][j] = g[n-1-j][i];
            g[n-1-j][i] = g[n-1-i][n-1-j];
            g[n-1-i][n-1-j] = g[j][n-1-i];
            g[j][n-1-i] = tmp;
        }
    }
}


void tumble(grid &g) {
    pull_up(g);
    rot_90_cw(g);
    pull_up(g);
    rot_90_cw(g);
    pull_up(g);
    rot_90_cw(g);
    pull_up(g);
    rot_90_cw(g);
}


int score(grid &g) {
    int load = 0;
    for (size_t i=0; i<g.size(); ++i) {
        for (size_t j=0; j<g[0].size(); ++j) {
            if (g[i][j] == 'O') {
                load += g.size()-i;
            }
        }
    }
    return load;
}


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    grid g;
    while (std::getline(file, line)) {
        g.push_back(line);
    }

    std::map<grid, int> visited;
    uint64_t i=0;
    for (; i<1000000000; ++i) {
        tumble(g);
        if (visited.contains(g)) break;
        visited[g] = i;
    }
    uint64_t cycle_len = i - visited[g];
    uint64_t cycle_idx = (1000000000-(i+1)) % cycle_len;
    std::cout << "cycle_len: " << cycle_len << '\n';
    std::cout << "cycle_idx: " << cycle_idx << '\n';

    for (int c=0; c<cycle_idx; ++c) tumble(g);
    for (auto l:g) std::cout << l << '\n';

    int64_t load = score(g);
    std::cout << "Load: " << load << std::endl;
    ans = load;

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}