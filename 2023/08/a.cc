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
    while (std::getline(file, line)) {
        std::string node = line.substr(0, 3);
        std::string node_l = line.substr(7, 3);
        std::string node_r = line.substr(12, 3);
        NODE_MAP[node] = std::make_pair(node_l, node_r);
    }
    std::cout << "Node ct: " << NODE_MAP.size() << std::endl;

    int i=0;
    std::string curr = "AAA";
    while (true) {
        ++ans;
        char ins = instructions[i];
        std::cout << curr << ", " << ins << std::endl;
        if (ins == 'L') {
            curr = NODE_MAP[curr].first;
        } else if (ins == 'R') {
            curr = NODE_MAP[curr].second;
        } else {
            throw std::domain_error("invalid instruction");
        }
        if (curr == "ZZZ") {
            break;
        }
        i = (i+1) % instructions.size();
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}