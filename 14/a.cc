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
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>


template <class K, class V>
class LinkedHashMap {
    private:
        typedef typename std::list<K>::iterator list_iterator;
        std::unordered_map<K, std::pair<V, list_iterator>> hash;
        std::list<K> ls;
    public:
        const int size() { return hash.size(); }
        V& operator[](K const& key) {
            if (!hash.contains(key)) {
                auto const it = ls.insert(ls.end(), key);
                hash.insert(std::make_pair(std::move(key), std::make_pair(std::move(V()), it)));
            }
            return hash.at(key).first;
        }
        K front_key() {
            return ls.front();
        }
        K back_key() {
            return ls.back();
        }
        V& front() {
            return (*this)[front_key()];
        }
        V& back() {
            return (*this)[back_key()];
        }
        std::list<K> keys() {
            return ls;
        }
};


typedef std::string row;
typedef std::vector<row> grid;
typedef LinkedHashMap<size_t, size_t> column_counter;  // row_start->rock_ct


int64_t arithmetic_series(int start, int end) {
    return end*(end+1)/2 - start*(start+1)/2;
}


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    grid g;
    while (std::getline(file, line)) {
        g.push_back(line);
    }

    int64_t load = 0;
    std::map<size_t, column_counter> col_to_counter;
    for (size_t i=0; i<g.size(); ++i) {
        for (size_t j=0; j<g[0].size(); ++j) {
            if (g[i][j] == 'O') {
                ++col_to_counter[j].back();
            }
            if (g[i][j] == '#') {
                col_to_counter[j][i+1] = 0;
            }
        }
    }

    for (auto& counter: col_to_counter) {
        for (auto& row_start: counter.second.keys()) {
            auto count = counter.second[row_start];
            auto inc = arithmetic_series(g.size()-row_start-count, g.size()-row_start);
            load += inc;
            if (count != 0) {
                std::cout << "(col, row, qty): (" << counter.first << ", " << row_start
                    << ", " << count << ") contributed " << inc << std::endl;
            }
        }
    }

    std::cout << "Load: " << load << std::endl;
    ans = load;

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}