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


struct Restriction;
typedef std::string workflow;
typedef std::string op;
typedef std::string xmas;
typedef int condition_value;
typedef std::tuple<xmas, op, condition_value> condition;
typedef std::pair<condition, workflow> branch;
typedef std::vector<branch> branches;
typedef std::pair<int, int> interval;
typedef std::pair<Restriction, workflow> Node;
const condition DEFAULT = std::make_tuple("default", "=", 0);
const workflow ACCEPT = "A";
const workflow REJECT = "R";
const workflow START = "in";

struct Restriction {
    interval x = std::make_pair(1, 4000);
    interval m = std::make_pair(1, 4000);
    interval a = std::make_pair(1, 4000);
    interval s = std::make_pair(1, 4000);
    auto xmas_get(xmas xm) {
        if (xm == "x") return x;
        if (xm == "m") return m;
        if (xm == "a") return a;
        if (xm == "s") return s;
        throw std::domain_error("bad xm, get");
    }
    auto xmas_set(xmas xm, interval xm_interval) {
        if (xm == "x") {
            x = xm_interval;
            return;
        }
        if (xm == "m") {
            m = xm_interval;
            return;
        }
        if (xm == "a") {
            a = xm_interval;
            return;
        }
        if (xm == "s") {
            s = xm_interval;
            return;
        }
        throw std::domain_error("bad xm, set");
    }
};

std::optional<interval> intersect_intervals(interval a, interval b) {
    auto [a1, a2] = a;
    auto [b1, b2] = b;
    interval intersection = std::make_pair(std::max(a1, b1), std::min(a2, b2));
    if (intersection.first > intersection.second) return std::nullopt;
    return intersection;
}

int64_t calc_combinations(Restriction& r) {
    int64_t ans = 1;
    for (auto xm : {r.x, r.m, r.a, r.s}) {
        auto [start, end] = xm;
        ans *= (end-start+1);
    }
    return ans;
}

int64_t explore(std::map<workflow, branches>& ins, std::deque<Node>& queue, Restriction& r, workflow w) {
    if (w == ACCEPT) return calc_combinations(r);
    if (w == REJECT) return 0;

    // std::cout << w << std::endl;
    for (auto& [cond, w_out] : ins[w]) {
        auto [c_xmas, c_op, c_val] = cond;
        if (cond == DEFAULT) {
            w = w_out;
            break;
        };
        interval r_xm = r.xmas_get(c_xmas);
        std::optional<interval> intersection;
        if (c_op == "<") {
            // make true node (new w)
            intersection = intersect_intervals(r_xm, std::make_pair(0, c_val-1));
            if (intersection.has_value()) {
                Restriction r_new = r;
                r_new.xmas_set(c_xmas, intersection.value());
                queue.push_back(std::make_pair(r_new, w_out));
            }

            // make false node (update in place and move to next branch)
            intersection = intersect_intervals(r_xm, std::make_pair(c_val, 4000));
            if (intersection.has_value()) {
                r.xmas_set(c_xmas, intersection.value());
                continue;
            }
        } else if (c_op == ">") {
            // make true node (new w)
            intersection = intersect_intervals(r_xm, std::make_pair(c_val+1, 4000));
            if (intersection.has_value()) {
                Restriction r_new = r;
                r_new.xmas_set(c_xmas, intersection.value());
                queue.push_back(std::make_pair(r_new, w_out));
            }

            // make false node (update in place and move to next branch)
            intersection = intersect_intervals(r_xm, std::make_pair(0, c_val));
            if (intersection.has_value()) {
                r.xmas_set(c_xmas, intersection.value());
                continue;
            }
        } else throw std::domain_error("invalid c_op");
    }
    queue.push_back(std::make_pair(r, w));
    return 0;
}

int64_t branch_out(std::map<workflow, branches>& ins, std::deque<Node>& queue) {
    int64_t ans = 0;
    while (!queue.empty()) {
        auto [r, w] = queue.front();
        queue.pop_front();
        ans += explore(ins, queue, r, w);
    }
    return ans;
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
    std::cout << "Ins ct: " << instructions.size() << std::endl;

    Restriction r;
    std::deque<Node> queue;
    queue.push_back(std::make_pair(r, START));
    ans = branch_out(instructions, queue);
    
    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}