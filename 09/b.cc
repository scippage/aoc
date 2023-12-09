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
        // is the [alternating] sum of the first elements of each iteration.
        std::vector<int> first_vals;
        first_vals.push_back(values.front());
        while (true) {
            std::vector<int> diffs;
            std::adjacent_difference(values.begin(), values.end(), std::back_inserter(diffs));
            diffs.erase(diffs.begin());
            first_vals.push_back(diffs.front());

            // for (auto& d : diffs) {
            //     std::cout<<d<<" ";
            // }
            // std::cout<<"\n";
            
            if (std::all_of(diffs.begin(), diffs.end(), [](int d){return d == 0;})) {
                int sign = -1;
                ans += std::accumulate(first_vals.begin(), first_vals.end(), 0,
                    [&sign](int a, int v){
                        sign *= -1;
                        return a + sign*v;
                    });
                break;
            }
            values = diffs;
        }
        for (auto& f : first_vals) {
            std::cout<<f<<" ";
        }
        std::cout<<"\n";
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}