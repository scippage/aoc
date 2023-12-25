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

void follow_loop(coords S_coords, coords prev_coords, coords curr_coords, std::set<coords> &path) {
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

    path.insert(curr_coords);
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
    std::set<coords> path;
    std::vector<dxdy> to_visit = {
        std::make_pair(0,-1), std::make_pair(0,1), std::make_pair(-1,0), std::make_pair(1,0)
    };
    while (!to_visit.empty()) {
        dxdy direction = to_visit.back();
        to_visit.pop_back();

        coords candidate_coords = std::make_pair(S_coords.first - direction.second, S_coords.second + direction.first);
        follow_loop(S_coords, S_coords, candidate_coords, path);
        if (path.size() != 0) {
            path.insert(S_coords);
            break;
        };
    }
    for (auto& c: path) {
        printf("(%d,%d) [%c]\n", c.first, c.second, maze[c.first][c.second]);
    }

    // replace S with the appropriate connecting pipe shape
    coords S_up = std::make_pair(S_coords.first-1, S_coords.second);
    coords S_down = std::make_pair(S_coords.first+1, S_coords.second);
    coords S_left = std::make_pair(S_coords.first, S_coords.second-1);
    coords S_right = std::make_pair(S_coords.first, S_coords.second+1);
    bool conn_up = is_valid(S_up) && (
        maze[S_up.first][S_up.second] == '|' ||
        maze[S_up.first][S_up.second] == 'F' ||
        maze[S_up.first][S_up.second] == '7'
    );
    bool conn_down = is_valid(S_down) && (
        maze[S_down.first][S_down.second] == '|' ||
        maze[S_down.first][S_down.second] == 'J' ||
        maze[S_down.first][S_down.second] == 'L'
    );
    bool conn_left = is_valid(S_left) && (
        maze[S_left.first][S_left.second] == '-' ||
        maze[S_left.first][S_left.second] == 'F' ||
        maze[S_left.first][S_left.second] == 'L'
    );
    bool conn_right = is_valid(S_right) && (
        maze[S_right.first][S_right.second] == '-' ||
        maze[S_right.first][S_right.second] == '7' ||
        maze[S_right.first][S_right.second] == 'J'
    );
    // 4C2 = 6
    char S_pipe;
    if (conn_up && conn_down) S_pipe='|';
    else if (conn_up && conn_left) S_pipe='J';
    else if (conn_up && conn_right) S_pipe='L';
    else if (conn_left && conn_right) S_pipe='-';
    else if (conn_down && conn_left) S_pipe='7';
    else if (conn_down && conn_right) S_pipe='F';
    else throw std::domain_error("bad S_pipe");
    std::cout << "S is a " << S_pipe << std::endl;
    maze[S_coords.first][S_coords.second] = S_pipe;

    // raycast from each point to right edge, counting pipe crossings
    // to avoid tangents, act like points start in the bottom left corner of each pipe
    for (int i=0; i<maze.size(); ++i) {
        for (int j=0; j<maze[0].size(); ++j) {
            if (path.contains(std::make_pair(i, j))) continue;

            // cast right
            int crossings = 0;
            for (int jj=j+1; jj<maze[0].size(); ++jj) {
                if (!path.contains(std::make_pair(i, jj))) continue;
                char pipe = maze[i][jj];
                if (pipe == '|' || pipe == 'F' || pipe == '7') {
                    // printf("origin (%d, %d) crossing (%d,%d) [%c]\n", i, j, i, jj, pipe);
                    ++crossings;
                }
            }
            if (crossings % 2 == 1) {
                printf("origin (%d, %d)\n", i, j);
                ++ans;
            }
        }
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}