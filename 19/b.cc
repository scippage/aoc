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
    std::list<interval> x = {std::make_pair(1, 4000)};
    std::list<interval> m = {std::make_pair(1, 4000)};
    std::list<interval> a = {std::make_pair(1, 4000)};
    std::list<interval> s = {std::make_pair(1, 4000)};
    auto xmas_get(xmas xm) {
        if (xm == "x") return x;
        if (xm == "m") return m;
        if (xm == "a") return a;
        if (xm == "s") return s;
        throw std::domain_error("bad xm, get");
    }
    auto xmas_set(xmas xm, std::list<interval> xm_list) {
        if (xm == "x") {
            x = xm_list;
            return;
        }
        if (xm == "m") {
            m = xm_list;
            return;
        }
        if (xm == "a") {
            a = xm_list;
            return;
        }
        if (xm == "s") {
            s = xm_list;
            return;
        }
        throw std::domain_error("bad xm, set");
    }
};

int64_t calc_combinations(Restriction& r) {
    int64_t ans = 1;
    for (std::list<interval> xm : {r.x, r.m, r.a, r.s}) {
        int64_t sum = 0;
        for (auto& [start, end] : xm) {
            sum += end-start+1;
        }
        ans *= sum;
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
        std::list<interval> r_xm = r.xmas_get(c_xmas);
        std::list<interval> xm_new;
        if (c_op == "<") {
            // make true node (new w)
            xm_new.clear();
            for (auto inv : r_xm) {
                auto [start, end] = inv;
                if (end < c_val) {
                    xm_new.push_back(inv);
                } else if (start < c_val && end >= c_val) {
                    xm_new.push_back(std::make_pair(start, c_val-1));
                }
            }
            if (!xm_new.empty()) {
                Restriction r_new = r;
                r_new.xmas_set(c_xmas, xm_new);
                queue.push_back(std::make_pair(r_new, w_out));
            };

            // make false node (update in place and move to next branch)
            xm_new.clear();
            for (auto inv : r_xm) {
                auto [start, end] = inv;
                if (start >= c_val) {
                    xm_new.push_back(inv);
                } else if (start < c_val && end >= c_val) {
                    xm_new.push_back(std::make_pair(c_val, end));
                }
            }
            if (!xm_new.empty()) {
                r.xmas_set(c_xmas, xm_new);
                continue;
            };
        } else if (c_op == ">") {
            // make true node (new w)
            xm_new.clear();
            for (auto inv : r_xm) {
                auto [start, end] = inv;
                if (start > c_val) {
                    xm_new.push_back(inv);
                } else if (start < c_val && end > c_val) {
                    xm_new.push_back(std::make_pair(c_val+1, end));
                }
            }
            if (!xm_new.empty()) {
                Restriction r_new = r;
                r_new.xmas_set(c_xmas, xm_new);
                queue.push_back(std::make_pair(r_new, w_out));
            };

            // make false node (update in place and move to next branch)
            xm_new.clear();
            for (auto inv : r_xm) {
                auto [start, end] = inv;
                if (end <= c_val) {
                    xm_new.push_back(inv);
                } else if (start <= c_val && end > c_val) {
                    xm_new.push_back(std::make_pair(start, c_val));
                }
            }
            if (!xm_new.empty()) {
                r.xmas_set(c_xmas, xm_new);
                continue;
            };
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