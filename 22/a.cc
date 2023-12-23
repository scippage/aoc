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

class Brick;
typedef std::shared_ptr<Brick> BrickPtr;

class Brick {
    public:
        int x1, y1, z1;
        int x2, y2, z2;
        std::string name;
        std::set<BrickPtr> supports;
        std::set<BrickPtr> dependers;

        bool operator<(const Brick& other) const {
            return std::min(z1, z2) < std::min(other.z1, other.z2);
        }

        Brick(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, std::string _name) 
            : x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2), name(_name) {};
};

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::vector<std::shared_ptr<Brick>> bricks;
    while (std::getline(file, line)) {
        auto brick_spec = Util::split(line, "~");
        auto brick_coords1 = Util::split(brick_spec[0], ",");
        auto brick_coords2 = Util::split(brick_spec[1], ",");
        auto brick_ptr = std::make_shared<Brick>(
            std::stoi(brick_coords1[0]),
            std::stoi(brick_coords1[1]),
            std::stoi(brick_coords1[2]),
            std::stoi(brick_coords2[0]),
            std::stoi(brick_coords2[1]),
            std::stoi(brick_coords2[2]),
            line
        );
        bricks.push_back(brick_ptr);
    }
    std::sort(bricks.begin(), bricks.end(), [](BrickPtr a, BrickPtr b) {return (*a < *b);});

    int max_x = 0, max_y = 0, max_z = 0;
    for (auto& brick_ptr: bricks) {
        auto brick = *brick_ptr;
        max_x = std::max(max_x, brick.x1);
        max_x = std::max(max_x, brick.x2);
        max_y = std::max(max_y, brick.y1);
        max_y = std::max(max_y, brick.y2);
        max_z = std::max(max_z, brick.z1);
        max_z = std::max(max_z, brick.z2);
        std::cout << brick.x1 << "," << brick.y1 << "," << brick.z1 << "~";
        std::cout << brick.x2 << "," << brick.y2 << "," << brick.z2 << std::endl;
    }
    std::cout << "Maxes: " << max_x << "," << max_y << "," << max_z << std::endl;
    std::vector<std::vector<int>> heights(max_x+1, std::vector<int>(max_y+1, 0));
    std::vector<std::vector<BrickPtr>> height_bricks(max_x+1, std::vector<BrickPtr>(max_y+1));

    for (auto& brick_ptr: bricks) {
        auto b = *brick_ptr;
        int curr_height = 0;
        std::vector<BrickPtr> supporters;
        for (int i=std::min(b.x1, b.x2); i<std::max(b.x1, b.x2)+1; ++i) {
            for (int j=std::min(b.y1, b.y2); j<std::max(b.y1, b.y2)+1; ++j) {
                if (heights[i][j] > curr_height) {
                    curr_height = heights[i][j];
                    supporters.clear();
                }
                if (heights[i][j] == curr_height) {
                    BrickPtr support_ptr = height_bricks[i][j];
                    supporters.push_back(support_ptr);
                }
            }
        }
        for (auto& support_ptr : supporters) {
            if (support_ptr.get()) {
                (support_ptr->dependers).insert(brick_ptr);
                (brick_ptr->supports).insert(support_ptr);
            }
        }
        for (int i=std::min(b.x1, b.x2); i<std::max(b.x1, b.x2)+1; ++i) {
            for (int j=std::min(b.y1, b.y2); j<std::max(b.y1, b.y2)+1; ++j) {
                heights[i][j] = curr_height + (std::max(b.z1, b.z2) - std::min(b.z1, b.z2) + 1);
                height_bricks[i][j] = brick_ptr;
            }
        }
    }

    // for (int i=0; i<heights.size(); ++i) {
    //     for (int j=0; j<heights[0].size(); ++j) {
    //         printf("(%d, %d)=%d\n", i, j, heights[i][j]);
    //     }
    // }

    for (auto& brick_ptr: bricks) {
        bool can_remove = true;
        for (auto& dep_ptr: brick_ptr->dependers) {
            if ((dep_ptr->supports).size() == 1) {
                can_remove = false;
            }
        }
        if (can_remove) ++ans;
    }

    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}