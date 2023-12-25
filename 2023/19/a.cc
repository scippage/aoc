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



typedef std::string workflow;
typedef std::string op;
typedef std::string xmas;
typedef int condition_value;
typedef std::tuple<xmas, op, condition_value> condition;
typedef std::pair<condition, workflow> branch;
typedef std::vector<branch> branches;
const condition DEFAULT = std::make_tuple("default", "=", 0);
const workflow ACCEPT = "A";
const workflow REJECT = "R";
const workflow START = "in";

struct part {
    int x;
    int m;
    int a;
    int s;
    part(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {}
    int xmas_to_int(xmas xm) {
        if (xm == "x") return x;
        if (xm == "m") return m;
        if (xm == "a") return a;
        if (xm == "s") return s;
        return -1;
    }
};

bool should_accept(workflow w) {
    return w == ACCEPT;
}

bool valid_part(std::map<workflow, branches>& ins, part& p) {
    workflow w = START;
    while (true) {
        for (auto& [cond, w_out] : ins[w]) {
            auto [c_xmas, c_op, c_val] = cond;
            if (cond == DEFAULT) {
                w = w_out;
                break;
            };
            int p_xmas = p.xmas_to_int(c_xmas);
            if (c_op == ">") {
                if (p_xmas > c_val) {
                    w = w_out;
                    break;
                } else continue;
            } else if (c_op == "<") {
                if (p_xmas < c_val) {
                    w = w_out;
                    break;
                } else continue;
            } else throw std::domain_error("invalid c_op");
        }
        if (w == ACCEPT || w == REJECT) return should_accept(w);
    }
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::map<workflow, branches> instructions;
    while (std::getline(file, line)) {
        if (line.empty()) break;
        workflow w = line.substr(0, line.find("{"));
        std::string rest = line.substr(line.find("{")+1);
        rest.pop_back();
        while (rest.find(",") != std::string::npos) {
            std::string b_str = rest.substr(0, rest.find(","));
            rest = rest.substr(rest.find(",")+1);
            std::string c_str = b_str.substr(0, b_str.find(":"));
            condition c = DEFAULT;
            if (c_str.find_first_of("><") != std::string::npos) {
                xmas c_xmas = c_str.substr(0, c_str.find_first_of("><"));
                op c_op = c_str.substr(c_str.find_first_of("><"), 1);
                condition_value c_value = std::stoi(c_str.substr(c_str.find_first_of("><")+1));
                c = std::make_tuple(c_xmas, c_op, c_value);
            }
            workflow ret_w = b_str.substr(b_str.find(":")+1);
            branch b = std::make_pair(c, ret_w);
            instructions[w].push_back(b);
        }
        branch default_b = std::make_pair(DEFAULT, rest);
        instructions[w].push_back(default_b);
    }

    std::vector<part> parts;
    while (std::getline(file, line)) {
        line = line.substr(line.find("=")+1);
        int x = std::stoi(line.substr(0, line.find(",")));
        line = line.substr(line.find("=")+1);
        int m = std::stoi(line.substr(0, line.find(",")));
        line = line.substr(line.find("=")+1);
        int a = std::stoi(line.substr(0, line.find(",")));
        line = line.substr(line.find("=")+1);
        int s = std::stoi(line.substr(0, line.find("}")));
        part p(x, m, a, s);
        parts.push_back(p);
    }

    std::cout << "Ins ct: " << instructions.size() << std::endl;
    std::cout << "Part ct: " << parts.size() << std::endl;

    // for (auto& [w, bs]: instructions) {
    //     std::cout << w << std::endl;
    //     for (auto& [c, w_n]: bs) {
    //         auto [c_xmas, c_op, c_v] = c;
    //         std::cout << c_xmas << c_op << c_v << " | " << w_n << std::endl;
    //     }
    // }
    // for (auto& [x, m, a, s]: parts) {
    //     std::cout << x << " " << m << " " << a << " " << s << std::endl;
    // }
    
    for (auto& p: parts) {
        if (valid_part(instructions, p)) {
            ans += p.x + p.m + p.a + p.s;
        }
    }
    
    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}