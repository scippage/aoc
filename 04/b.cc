#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>


class Entry {
    public:
        int card_num;
        std::vector<int> winners;
        std::vector<int> numbers;  
        Entry(int card_num, std::vector<int> winners, std::vector<int> numbers) {
            this->card_num = card_num;
            this->winners = winners;
            this->numbers = numbers;
        }
};

std::vector<Entry> entries;
std::map<int, int> collection;  // (card_idx, count)

int process_entry(Entry *entry) {
    int matches = 0;
    std::set<int> set;
    for (int winner : entry->winners) {
        set.insert(winner);
    }
    for (int number : entry->numbers) {
        if (set.find(number) != set.end()) {
            matches++;
        }
    }
    return matches;
}

int process_collection() {
    int total_cards = 0;
    for (int i=0; i<collection.size(); i++) {
        total_cards += collection[i];
        int matches = process_entry(&entries[i]);
        for (int j=i+1; j<i+1+matches; j++) {
            collection[j] += collection[i];
        }
    }
    return total_cards;
}

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    while (std::getline(file, line)) {
        int card_num = std::stoi(line.substr(5, 3));
        std::cout << card_num << " | ";

        std::string rest = line.substr(line.find(": ") + 2, std::string::npos);
        std::string winners_str = rest.substr(0, rest.find(" | "));
        std::vector<int> winners;
        for (int i=0; i < winners_str.size(); i+=3) {
            std::string winner = winners_str.substr(i, 2);
            std::cout << winner << " ";
            winners.push_back(std::stoi(winner));
        }
        std::cout << "| ";
        
        std::string numbers_str = rest.substr(rest.find(" | ") + 3, std::string::npos);
        std::vector<int> numbers;
        for (int i=0; i < numbers_str.size(); i+=3) {
            std::string number = numbers_str.substr(i, 2);
            std::cout << number << " ";
            numbers.push_back(std::stoi(number));
        }
        std::cout << std::endl;

        Entry entry(card_num, winners, numbers);
        collection[card_num-1] = 1;
        entries.push_back(entry);
    }
    std::cout << "Entries: " << entries.size() << std::endl;
    if (file.is_open()) {
        file.close();
    }

    int ans = process_collection();

    printf("Ans: %d\n", ans);
    exit(EXIT_SUCCESS);
}