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
        void erase(K const& key) {
            auto const p = hash.find(key);
            if (p != hash.end()) {
                ls.erase(p->second.second);
                hash.erase(p);
            }
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
        class Iterator {
            private:
                typename std::list<K>::iterator it;
                std::list<K> l;
                std::unordered_map<K, std::pair<V, list_iterator>> h;
            public:
                Iterator(typename std::list<K>::iterator iterator,
                    std::list<K>& ls,
                    std::unordered_map<K, std::pair<V, list_iterator>>& hash) : it(iterator), l(ls), h(hash) {}
                std::pair<const K, V> operator*() {
                    return std::make_pair(*it, h[*it].first);
                }
                Iterator& operator++() {
                    ++it;
                    return *this;
                }
                bool operator!=(const Iterator& other) const {
                    return it != other.it;
                }
            };
        Iterator begin() {
            return Iterator(ls.begin(), ls, hash);
        }
        Iterator end() {
            return Iterator(ls.end(), ls, hash);
        }
};


typedef int hash_t;
typedef int focal_len_t;
typedef std::string label_t;
typedef std::pair<label_t, focal_len_t> lens_t;
typedef LinkedHashMap<label_t, lens_t> box_t;


int hash(std::string token) {
	int res = 0;
	for (auto& c: token) {
		res += int(c);
		res *= 17;
		res %= 256;
	}
	return res;
}


int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::getline(file, line);
    std::istringstream iss(line);
    std::vector<std::string> instructions;
    std::string token;
    while (std::getline(iss, token, ',')) {
        instructions.push_back(token);
    }

    std::map<hash_t, box_t> boxes;
    for (auto& instruction: instructions) {
        label_t label = instruction.substr(0, instruction.find_first_of("=-"));
        std::string ins_args = instruction.substr(instruction.find_first_of("=-")+1);
        hash_t box_id = hash(label);
        if (ins_args.size() == 0){
            // - instruction
            boxes[box_id].erase(label);
        } else {
            // = instruction
            focal_len_t focal_len = std::stoi(ins_args);
            boxes[box_id][label] = std::make_pair(label, focal_len);
        }
    }

    for (auto& box_entry: boxes) {
        printf("Box %d\n", box_entry.first);
        int box_slot=1;
        for (auto lens_entry: box_entry.second) {
            lens_t lens = lens_entry.second;
            printf("[%s %d] ", lens.first.c_str(), lens.second);

            ans += (box_entry.first+1)*box_slot*lens.second;
            ++box_slot;
        }
        printf("\n");
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}