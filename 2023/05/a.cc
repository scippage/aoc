#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>


std::deque<std::int64_t> seeds;
typedef std::vector<std::tuple<std::int64_t, std::int64_t, std::int64_t>> gardenMap;
std::map<std::string, gardenMap> garden_maps;

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    
    // parse pre-map
    std::getline(file, line);
    std::istringstream iss(line.substr(7));
    std::string token;
    while (std::getline(iss, token, ' ')) {
        seeds.push_back(std::stoll(token));
    }
    for (const auto& seed : seeds) std::cout << seed << " ";
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
    std::deque<std::int64_t> garden_items = seeds;
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
        std::deque<std::int64_t> new_garden_items;
        for (auto& entry : map) {
            int64_t src = std::get<0>(entry);
            int64_t dest = std::get<1>(entry);
            int64_t range = std::get<2>(entry);

            for (auto it = garden_items.begin(); it != garden_items.end();) {
                auto garden_item = *it;
                if (garden_item >= src && garden_item < src+range) {
                    int64_t offset = garden_item - src;
                    new_garden_items.push_back(dest+offset);
                    // std::cout << "Remapped " << garden_item << "->" << dest+offset << " by " << src << "\n";
                    it = garden_items.erase(it);
                } else {
                    it++;
                }
            }
        }
        new_garden_items.insert(std::end(new_garden_items), garden_items.begin(), garden_items.end());
        std::cout << "Inserted " << garden_items.size() << " unmapped items, bringing total length to " << new_garden_items.size() << std::endl;
        garden_items = new_garden_items;
        new_garden_items.clear();
        source_type = destination_type;

        std::cout << "New elements: ";
        for (auto& garden_item : garden_items) {
            std::cout << garden_item  << " ";
        }
        std::cout << std::endl;
    }
    std::sort(garden_items.begin(), garden_items.end());

    if (file.is_open()) {
        file.close();
    }
    int64_t ans = garden_items[0];

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}