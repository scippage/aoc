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


typedef std::string pipes;
std::vector<pipes> maze;

typedef std::pair<int, int> coords;  // (i,j)
typedef std::pair<int, int> dxdy;  // standard cartesian
typedef std::vector<dxdy> connections;
std::unordered_map<char, connections> PIPES = {
    {'S', {std::make_pair(0,-1), std::make_pair(0,1), std::make_pair(-1,0), std::make_pair(1,0)}},
    {'|', {std::make_pair(0,-1), std::make_pair(0,1)}},
    {'-', {std::make_pair(-1,0), std::make_pair(1,0)}},
    {'L', {std::make_pair(0,1), std::make_pair(1,0)}},
    {'J', {std::make_pair(0,1), std::make_pair(-1,0)}},
    {'7', {std::make_pair(0,-1), std::make_pair(-1,0)}},
    {'F', {std::make_pair(0,-1), std::make_pair(1,0)}},
    {'.', {}}
};


int is_valid(coords c) {
    return (c.first >= 0 && c.first < maze.size()) && (c.second >= 0 && c.second < maze[1].size());
}

void follow_loop(coords S_coords, coords prev_coords, coords curr_coords, std::vector<coords> &path) {
    if (!is_valid(curr_coords)) {
        path.clear();
        return;
    }

    char pipe = maze[curr_coords.first][curr_coords.second];
    bool connected_to_prev = false;
    coords next_coords;
    for (auto& d: PIPES[pipe]) {
        coords c_new = std::make_pair(curr_coords.first - d.second, curr_coords.second + d.first);
        if (c_new == prev_coords) {
            connected_to_prev = true;
            continue;
        } else {
            next_coords = c_new;
        }
    }
    if (!connected_to_prev) {
        path.clear();
        return;
    }
    printf("Moving from (i,j)=(%d,%d) [%c] to (i,j)=(%d,%d)\n",
        curr_coords.first, curr_coords.second, pipe,
        next_coords.first, next_coords.second);

    path.push_back(curr_coords);
    if (next_coords == S_coords) return;
    return follow_loop(S_coords, curr_coords, next_coords, path);
}


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    while (std::getline(file, line)) {
        maze.push_back(line);
    }

    // find S
    coords S_coords;
    for (int i=0; i<maze.size(); ++i) {
        for (int j=0; j<maze[0].size(); ++j) {
            if (maze[i][j] == 'S') {
                S_coords = std::make_pair(i, j);
                goto foundS;
            }
        }
    }
    foundS:
    printf("S at (i,j)=(%d,%d)\n", S_coords.first, S_coords.second);

    // find loop
    std::vector<coords> path;
    std::vector<dxdy> to_visit = {
        std::make_pair(0,-1), std::make_pair(0,1), std::make_pair(-1,0), std::make_pair(1,0)
    };
    while (!to_visit.empty()) {
        dxdy direction = to_visit.back();
        to_visit.pop_back();

        coords candidate_coords = std::make_pair(S_coords.first - direction.second, S_coords.second + direction.first);
        follow_loop(S_coords, S_coords, candidate_coords, path);
        if (path.size() != 0) {
            path.push_back(S_coords);
            break;
        };
    }
    for (auto& c: path) {
        printf("(%d,%d) [%c]\n", c.first, c.second, maze[c.first][c.second]);
    }

    // from online: can use shoelace formula together with Pick's theorem
    int area = 0;
    for (size_t i=0; i<path.size(); ++i) {
        // det([path[i], path[i+1]])
        area += path[i].first*path[(i+1)%path.size()].second - path[i].second*path[(i+1)%path.size()].first;
        // printf("(%d,%d)-(%d,%d)\n", path[i].first, path[i].second, path[(i+1)%path.size()].first, path[(i+1)%path.size()].second);
    }
    area *= 0.5;
    area = abs(area);

    // Pick's: A = i + b/2 - 1
    ans = area + 1 - (path.size()/2);

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}