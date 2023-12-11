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
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>


typedef std::pair<int, int> coords;
typedef std::string row;
std::vector<row> world;

int64_t compute_dist(coords a, coords b, std::set<int> nonempty_rows, std::set<int> nonempty_cols) {
    int64_t Di = 0;
    int64_t Dj = 0;
    for (int i=std::min(a.first, b.first); i<std::max(a.first, b.first); ++i) {
        ++Di;
        if (!nonempty_rows.contains(i)) Di += 1;
    }
    for (int j=std::min(a.second, b.second); j<std::max(a.second, b.second); ++j) {
        ++Dj;
        if (!nonempty_cols.contains(j)) Dj += 1;
    }
    // printf("(%d,%d)->(%d,%d) is dist (%d,%d)\n", a.first, a.second, b.first, b.second, Di, Dj);
    return Di+Dj;
}


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::set<int> nonempty_rows;
    std::set<int> nonempty_cols;
    std::vector<coords> galaxy_locs;
    for (int i=0; std::getline(file, line); ++i) {
        world.push_back(line);
        bool empty_row = true;
        for (int j=0; j<line.size(); ++j) {
            if (line[j] == '#') {
                galaxy_locs.push_back(std::make_pair(i, j));
                nonempty_cols.insert(j);
                empty_row = false;
            }
        }
        if (!empty_row) nonempty_rows.insert(i);
    }

    for (auto &l: galaxy_locs) printf("(i,j)=(%d,%d)\n", l.first, l.second);
    printf("len: %zd\n", galaxy_locs.size());

    for (int a=0; a<galaxy_locs.size(); ++a) {
        for (int b=a+1; b<galaxy_locs.size(); ++b) {
            ans += compute_dist(galaxy_locs[a], galaxy_locs[b], nonempty_rows, nonempty_cols);
        }
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}