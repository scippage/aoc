#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <sstream>
#include <vector>

typedef std::pair<int64_t, int64_t> interval;
std::list<interval> seed_ranges;
typedef std::vector<std::tuple<int64_t, int64_t, int64_t>> gardenMap;
std::map<std::string, gardenMap> garden_maps;

std::optional<interval> find_overlap(interval candidate, interval reference) {
    int64_t left = std::max(reference.first, candidate.first);
    int64_t right = std::min(reference.second, candidate.second);
    if (right < left) return std::nullopt;
    return std::make_pair(left, right);
}

std::stringstream pp_interval(interval i) {
    std::stringstream buffer;
    buffer << "[" << i.first << ", " << i.second << "]";
    return buffer;
}

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    
    // parse pre-map
    std::getline(file, line);
    std::istringstream iss(line.substr(7));
    std::string token;
    while (std::getline(iss, token, ' ')) {
        int64_t start = std::stoll(token);
        std::getline(iss, token, ' ');
        int64_t end = start+std::stoll(token)-1;
        seed_ranges.push_back(std::make_pair(start, end));
    }
    for (const auto& seed_range : seed_ranges) {
        std::cout << pp_interval(seed_range).str() << " ";
    }
    std::cout << std::endl;
    std::getline(file, line);

    // parse maps
    while (std::getline(file, line)) {
        std::string map_name = line.substr(0, line.find(" map:"));
        gardenMap map;
        // std::cout << "Parsing map " << map_name << std::endl;
        while (std::getline(file, line)) {
            if (line.empty()) break;
            std::istringstream iss(line);
            std::string dest;
            std::string src;
            std::string range;
            std::getline(iss, dest, ' ');
            std::getline(iss, src, ' ');
            std::getline(iss, range, ' ');
            map.push_back(std::make_tuple(std::stoll(src), std::stoll(dest), std::stoll(range)));
        }
        garden_maps[map_name] = map;
    }
    std::cout << "Number of maps: " << garden_maps.size() << std::endl;

    // map seeds to locations
    std::string source_type = "seed";
    std::string destination_type;
    std::list<interval> garden_items = seed_ranges;
    while (destination_type != "location") {
        gardenMap map;
        for (auto& entry : garden_maps) {
            std::string key = entry.first;
            if (key.substr(0, key.find("-to-")) == source_type) {
                destination_type = key.substr(key.find("-to-") + 4);
                map = entry.second;
                break;
            }
        }
        std::cout << "Mapping " << source_type << "->" << destination_type << std::endl;
        std::list<interval> new_garden_items;
        for (auto& entry : map) {
            int64_t src = std::get<0>(entry);
            int64_t dest = std::get<1>(entry);
            int64_t range = std::get<2>(entry);

            for (auto it = garden_items.begin(); it != garden_items.end();) {
                auto garden_item = *it;
                std::optional<interval> overlap = find_overlap(garden_item, std::make_pair(src, src+range-1));
                if (overlap.has_value()) {
                    //   [ ]   overlap
                    // [     ] original
                    // readd unmappable interval
                    if (garden_item.first < overlap.value().first) {
                        garden_items.insert(it, std::make_pair(garden_item.first, overlap.value().first-1));
                    }
                    if (garden_item.second > overlap.value().second) {
                        garden_items.insert(it, std::make_pair(overlap.value().second+1, garden_item.second));
                    }

                    // add mapped interval
                    overlap.value().first += dest-src;
                    overlap.value().second += dest-src;
                    new_garden_items.push_back(overlap.value());
                    std::cout << "Remapped " << pp_interval(garden_item).str() << "->" << pp_interval(overlap.value()).str() 
                              << " by " << src << "\n";
                    it = garden_items.erase(it);
                } else {
                    it++;
                }
            }
        }
        new_garden_items.insert(new_garden_items.end(), garden_items.begin(), garden_items.end());
        std::cout << "Inserted " << garden_items.size() << " unmapped items, bringing total length to " << new_garden_items.size() << std::endl;
        garden_items = new_garden_items;
        new_garden_items.clear();
        source_type = destination_type;

        std::cout << "New intervals: ";
        for (auto& garden_item : garden_items) {;
            std::cout << pp_interval(garden_item).str() << " ";
        }
        std::cout << std::endl;
    }
    garden_items.sort();

    if (file.is_open()) {
        file.close();
    }
    auto ans = pp_interval(garden_items.front()).str();

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}