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
#include "z3++.h"
#include "z3_api.h"
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

     /* 
        We have equations:

        x1 + v_{x1}(t1) = x* + v_{x*}(t1)
        y1 + v_{y1}(t1) = y* + v_{y*}(t1)
        z1 + v_{z1}(t1) = z* + v_{z*}(t1)
        x2 + v_{x2}(t2) = x* + v_{x*}(t2)
        ...
        zn + v_{zn}(tn) = z* + v_{z*}(tn)

        where we are trying to find integer values
        x*, y*, z*, v_{x*}, v_{y*}, v_{z*} where all t_i are positive.
        (the t_i don't have to be integers but we assume they are here)

        Because this problem has exactly one solution, 3 (noncollinear) hailstones are enough
        to fully specify the path of our thrown hailstone; we are solving 9 equations in 
        9 unknowns.

        Unfortunately I can't figure out how to make this system linear so we resort to using z3.
    */

    auto& c1 = coords[0];
    auto& c2 = coords[1];
    auto& c3 = coords[2];
    z3::context c;
    z3::expr px = c.int_const("px");
    z3::expr py = c.int_const("py");
    z3::expr pz = c.int_const("pz");
    z3::expr vx = c.int_const("vx");
    z3::expr vy = c.int_const("vy");
    z3::expr vz = c.int_const("vz");
    z3::expr T1 = c.int_const("T1");
    z3::expr T2 = c.int_const("T2");
    z3::expr T3 = c.int_const("T3");
    z3::solver s(c);
    s.add(T1 > 0);
    s.add(T2 > 0);
    s.add(T3 > 0);
    s.add((px-c.int_val(c1.x)) == -T1*(vx-c.int_val(c1.vx)));
    s.add((py-c.int_val(c1.y)) == -T1*(vy-c.int_val(c1.vy)));
    s.add((pz-c.int_val(c1.z)) == -T1*(vz-c.int_val(c1.vz)));
    s.add((px-c.int_val(c2.x)) == -T2*(vx-c.int_val(c2.vx)));
    s.add((py-c.int_val(c2.y)) == -T2*(vy-c.int_val(c2.vy)));
    s.add((pz-c.int_val(c2.z)) == -T2*(vz-c.int_val(c2.vz)));
    s.add((px-c.int_val(c3.x)) == -T3*(vx-c.int_val(c3.vx)));
    s.add((py-c.int_val(c3.y)) == -T3*(vy-c.int_val(c3.vy)));
    s.add((pz-c.int_val(c3.z)) == -T3*(vz-c.int_val(c3.vz)));
    std::cout << s << std::endl;
    std::cout << s.to_smt2() << std::endl;
    if (s.check() == z3::sat) std::cout << "satisfiable" << std::endl;
    z3::model model = s.get_model();
    std::cout << "px = " << model.eval(px) << std::endl;
    std::cout << "py = " << model.eval(py) << std::endl;
    std::cout << "pz = " << model.eval(pz) << std::endl;
    std::cout << "vx = " << model.eval(vx) << std::endl;
    std::cout << "vy = " << model.eval(vy) << std::endl;
    std::cout << "vz = " << model.eval(vz) << std::endl;
    std::cout << "T1 = " << model.eval(T1) << std::endl;
    std::cout << "T2 = " << model.eval(T2) << std::endl;
    std::cout << "T3 = " << model.eval(T3) << std::endl;
    
    int64_t px_res, py_res, pz_res;
    int64_t vx_res, vy_res, vz_res;
    Z3_get_numeral_int64(c, model.eval(px), &px_res);
    Z3_get_numeral_int64(c, model.eval(py), &py_res);
    Z3_get_numeral_int64(c, model.eval(pz), &pz_res);
    Z3_get_numeral_int64(c, model.eval(vx), &vx_res);
    Z3_get_numeral_int64(c, model.eval(vy), &vy_res);
    Z3_get_numeral_int64(c, model.eval(vz), &vz_res);
    ans = px_res + py_res + pz_res;

    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}