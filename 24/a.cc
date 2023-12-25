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
#include <Eigen/Dense>
#include "../util/util.h"

class Coord {
    public:
        int64_t x, y, z;
        int64_t vx, vy, vz;

        bool operator<(const Coord& other) const {
            return std::make_tuple(x, y, z) < std::make_tuple(other.x, other.y, other.z);
        }

        Coord(int64_t _x, int64_t _y, int64_t _z, int64_t _vx, int64_t _vy, int64_t _vz) 
            : x(_x), y(_y), z(_z), vx(_vx), vy(_vy), vz(_vz) {};
};

typedef std::vector<Coord> Coords;

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    Coords coords;
    while (std::getline(file, line)) {
        auto coord_spec = Util::split(line, " @ ");
        auto pos = Util::split(coord_spec[0], ", ");
        auto vel = Util::split(coord_spec[1], ", ");
        Coord coord = Coord(
            std::stoll(pos[0]), std::stoll(pos[1]), std::stoll(pos[2]),
            std::stoll(vel[0]), std::stoll(vel[1]), std::stoll(vel[2])
        );
        coords.push_back(coord);
    }

    int64_t l_bound = 200000000000000;
    int64_t u_bound = 400000000000000;
    // int64_t l_bound = 7;
    // int64_t u_bound = 27;

    for (int i=0; i<coords.size(); ++i) {
        for (int j=i+1; j<coords.size(); ++j) {
            auto& c1 = coords[i];
            auto& c2 = coords[j];
            /* 
                We need to check if rays (not projectiles!) intersect for positive times t1, t2.

                x1 + v_{x1}(t1) = x2 + v_{x2}(t2)
                y1 + v_{y1}(t1) = y2 + v_{y2}(t2)

                or

                [[v_{x1} -v_{x2}] [[t1]  = [[x2-x1]
                 [v_{y1} -v_{y2}]] [t2]]    [y2-y1]]

                The intersection will be at (x1+(t1)v_{x1}, y1+(t1)v_{y1}).
            */
            Eigen::Matrix2f A;
            Eigen::Vector2f b;
            A << c1.vx, -c2.vx, 
                 c1.vy, -c2.vy;
            b << c2.x-c1.x, c2.y-c1.y;
            Eigen::FullPivLU<Eigen::Matrix2f> lu(A);
            if (!lu.isInvertible()) continue;
            Eigen::Vector2f t = lu.solve(b);
            if (t[0] < 0 || t[1] < 0) continue;
            double intersect_x = c1.x + t[0]*c1.vx;
            double intersect_y = c1.y + t[0]*c1.vy;

            // printf("%f,%f\n", intersect_x, intersect_y);
            if (l_bound <= intersect_x && intersect_x <= u_bound &&
                l_bound <= intersect_y && intersect_y <= u_bound) ++ans;
        }
    }


    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}