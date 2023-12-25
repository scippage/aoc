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


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> values{std::istream_iterator<int>(iss), 
                                std::istream_iterator<int>()};
        // by tracing through an example, we can see that the next extrapolated val
        // is the sum of the last elements of each iteration
        
        std::vector<int> last_vals;
        last_vals.push_back(values.back());
        while (true) {
            std::vector<int> diffs;
            std::adjacent_difference(values.begin(), values.end(), std::back_inserter(diffs));
            diffs.erase(diffs.begin());
            last_vals.push_back(diffs.back());

            // for (auto& d : diffs) {
            //     std::cout<<d<<" ";
            // }
            // std::cout<<"\n";

            if (std::all_of(diffs.begin(), diffs.end(), [](int d){return d == 0;})) {
                ans += std::accumulate(last_vals.begin(), last_vals.end(), 0);
                break;
            }
            values = diffs;
        }
        // for (auto& l : last_vals) {
        //     std::cout<<l<<" ";
        // }
        // std::cout<<"\n";
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}