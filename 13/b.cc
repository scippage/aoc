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


template<typename T>
bool is_palindrome(std::vector<T> vec, int start_idx, int end_idx) {
    // we only accept even palindromes
    if (((end_idx-start_idx+1) % 2) != 0) return false;

    for (size_t i=start_idx; i<start_idx+(end_idx-start_idx+1)/2; ++i) {
        if (vec[i] != vec[end_idx-(i-start_idx)]) return false;
    }
    return true;
}


grid transpose_grid(grid g) {
    grid transpose(g[0].size());
    for (size_t i=0; i<g.size(); ++i) {
        for (size_t j=0; j<g[0].size(); ++j) {
            transpose[j] += g[i][j];
        }
    }
    return transpose;
}


int grid_summary(grid g, int multiplier, int excluded_val);
int try_smudge(grid g, size_t i, size_t j, int excluded_val) {
    int summary_val = 0;
    char smudge = g[i][j];
    char replacement = smudge == '#' ? '.' : '#';
    g[i][j] = replacement;

    summary_val += grid_summary(g, 100, excluded_val);
    grid transpose = transpose_grid(g);
    summary_val += grid_summary(transpose, 1, excluded_val);

    if (summary_val != 0) {
        printf("smudge at (%zu,%zu). excluded_val: %d. summary_val: %d\n", i, j, excluded_val, summary_val);
    }
    return summary_val;
}


int grid_summary(grid g, int multiplier, int excluded_val) {
    int summary_val = 0;
    for (size_t i=0; i<g.size(); ++i) {
        // check if row_ids[i:] is a palindrome
        if (is_palindrome(g, i, g.size()-1)) {
            summary_val = multiplier*(i+(g.size()-i)/2);
            if (summary_val == excluded_val) {
                summary_val = 0;
            } else {
                break;
            }
        }
        // check if row_ids[:-i] is a palindrome
        if (is_palindrome(g, 0, g.size()-1-i)) {
            summary_val = multiplier*((g.size()-i)/2);
            if (summary_val == excluded_val) {
                summary_val = 0;
            } else {
                break;
            }
        }
    }
    return summary_val;
}

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::vector<grid> grids;
    grid g;
    while (std::getline(file, line)) {
        if (line.empty()) {
            grids.push_back(g);
            g.clear();
        } else {
            g.push_back(line);
        }
    }
    grids.push_back(g);
    std::cout << "Grids: " << grids.size() << std::endl;

    int excluded_val = 0;
    for (auto& g: grids) {
        // check rows
        excluded_val += grid_summary(g, 100, -1);

        // check cols
        grid transpose = transpose_grid(g);
        excluded_val += grid_summary(transpose, 1, -1);

        for (size_t i=0; i<g.size(); ++i) {
            for (size_t j=0; j<g[0].size(); ++j) {
                ans += try_smudge(g, i, j, excluded_val);
            }
        }
        excluded_val = 0;
    }
    // every smudge can be mirrored
    ans /= 2;

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}