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


typedef std::pair<std::string, std::string> node_t;
node_t NODE;
std::unordered_map<std::string, node_t> NODE_MAP;

int main() {
    std::ifstream file("./input.txt");
    std::string instructions;
    std::string line;
    int64_t ans = 0;

    std::getline(file, instructions);
    std::getline(file, line);
    std::vector<std::string> curr_nodes;
    while (std::getline(file, line)) {
        std::string node = line.substr(0, 3);
        std::string node_l = line.substr(7, 3);
        std::string node_r = line.substr(12, 3);
        NODE_MAP[node] = std::make_pair(node_l, node_r);
        if (node[2] == 'A') curr_nodes.push_back(node);
    }
    std::cout << "Node ct: " << NODE_MAP.size() << std::endl;
    std::cout << "Starting nodes: ";
    for (auto& n: curr_nodes) std::cout << n << " ";
    std::cout << std::endl;
    
    // it seems like all cycles are independent (start/end at same nodes)
    // so find lcm?
    std::vector<int> cycle_lengths;
    for (auto& c: curr_nodes) {
        int i=0;
        int cycle_len=0;
        while (true) {
            ++cycle_len;
            char ins = instructions[i];
            if (ins == 'L') {
                c = NODE_MAP[c].first;
            } else if (ins == 'R') {
                c = NODE_MAP[c].second;
            } else {
                throw std::domain_error("invalid instruction");
            }
            if (c[2] == 'Z') {
                break;
            }
            i = (i+1) % instructions.size();
        }
        cycle_lengths.push_back(cycle_len);
    }
    for (auto& l: cycle_lengths) std::cout << l << std::endl;

    ans = std::accumulate(cycle_lengths.begin(), cycle_lengths.end(), (int64_t)1, 
        [](int64_t a, int64_t b){ return std::lcm(a, b);}
    );

    // what if cycles weren't independent?
    // 1A -> ... -> 1Z -> 2A -> ... 2Z
    // 9A -> ... -> 9Z -> 9A

    // std::set<std::pair<std::vector<std::string>, int>> visited;
    // int i=0;
    // while (true) {
    //     auto key = std::make_pair(curr_nodes, i);
    //     if (visited.contains(key)) {
    //         std::cout << "cycle found after iter " << ans << "\n";
    //         exit(EXIT_FAILURE);
    //     }
    //     visited.insert(key);
    //     ++ans;
    //     char ins = instructions[i];
    //     // std::cout << ins << ": ";
    //     // for (auto& c: curr_nodes) std::cout << c << " ";
    //     // std::cout << std::endl;
    //     if (log10(ans) == std::trunc(log10(ans))) std::cout << "Iter " << ans << std::endl;
    //     if (ins == 'L') {
    //         for (auto& c: curr_nodes) c = NODE_MAP[c].first;
    //     } else if (ins == 'R') {
    //         for (auto& c: curr_nodes) c = NODE_MAP[c].second;
    //     } else {
    //         throw std::domain_error("invalid instruction");
    //     }
    //     for (int i=0; i<curr_nodes.size(); ++i) {
    //         auto c = curr_nodes[i];
    //         if (c[2] == 'Z') {
    //             std::cout << "cycle " << i << " ended at " << c << " iter " << ans << std::endl;
    //         }
    //     }
    //     if (std::all_of(curr_nodes.begin(), curr_nodes.end(), [](std::string c){return (c[2] == 'Z');})) {
    //         break;
    //     }
    //     i = (i+1) % instructions.size();
    // }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}