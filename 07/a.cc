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
#include <string>
#include <string_view>
#include <sstream>
#include <vector>


std::string DECK = "23456789TJQKA";
std::unordered_map<char, int> CARD_RANKS;
std::unordered_map<std::string, int> HAND_RANKS = {
    {"hc", 1},
    {"1p", 2},
    {"2p", 3},
    {"3ok", 4},
    {"fh", 5},
    {"4ok", 6},
    {"5ok", 7}
};

struct Card {
    char card;
    Card(char c) : card(c) {}

    bool operator < (const Card& other) const {
        return (CARD_RANKS[card] < CARD_RANKS[other.card]);
    }
};

struct Hand {
    std::string cards;
    int bid;
    int rank;

    int compute_rank(std::string& cs) {
        std::unordered_map<char, int> card_counts;
        for (char& c: cs) {
            ++card_counts[c];
        }
        std::unordered_map<int, int> xok_counts;
        for (auto& card_count: card_counts) {
            ++xok_counts[card_count.second];
        }
        if (xok_counts[5] == 1) {
            rank = HAND_RANKS["5ok"];
        } else if (xok_counts[4] == 1) {
            rank = HAND_RANKS["4ok"];
        } else if (xok_counts[3] == 1 && xok_counts[2] == 1) {
            rank = HAND_RANKS["fh"];
        } else if (xok_counts[3] == 1) {
            rank = HAND_RANKS["3ok"];
        } else if (xok_counts[2] == 2) {
            rank = HAND_RANKS["2p"];
        } else if (xok_counts[2] == 1) {
            rank = HAND_RANKS["1p"];
        } else {
            rank = HAND_RANKS["hc"];
        }
        return rank;
    }

    Hand(std::string& cs, int b) : cards(cs), bid(b) {
        rank = compute_rank(cs);
    }

    bool operator < (const Hand& other) const {
        if (rank == other.rank) {
            for (int i=0; i<5; ++i) {
                if (cards[i] == other.cards[i]) {
                    continue;
                } else {
                    return Card(cards[i]) < Card(other.cards[i]);
                }
            }
            return 0;
        } else {
            return rank < other.rank;
        }
    }
};

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;

    int starting_rank = 1;
    for (char& c : DECK) {
        CARD_RANKS[c] = starting_rank;
        starting_rank++;
    }

    std::vector<Hand> hands;
    while (std::getline(file, line)) {
        std::string cards = line.substr(0, 5);
        int bid = std::stoi(line.substr(5, std::string::npos));
        hands.push_back(Hand(cards, bid));
    }

    std::sort(hands.begin(), hands.end(), std::less<Hand>());
    for (auto& hand : hands) std::cout << hand.cards << " " << hand.bid << "\n";
    std::cout << std::endl;

    for (int i=0; i<hands.size(); ++i) {
        ans += (i+1)*hands[i].bid;
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}