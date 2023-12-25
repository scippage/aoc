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


int grid_summary(grid g, int multiplier) {
    int summary_val = 0;
    for (size_t i=0; i<g.size(); ++i) {
        // check if row_ids[i:] is a palindrome
        if (is_palindrome(g, i, g.size()-1)) {
            summary_val = multiplier*(i+(g.size()-i)/2);
            break;
        }
        // check if row_ids[:-i] is a palindrome
        if (is_palindrome(g, 0, g.size()-1-i)) {
            summary_val = multiplier*((g.size()-i)/2);
            break;
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

    for (auto& g: grids) {
        // check rows
        ans += grid_summary(g, 100);

        // check cols
        grid transpose = transpose_grid(g);
        ans += grid_summary(transpose, 1);
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}