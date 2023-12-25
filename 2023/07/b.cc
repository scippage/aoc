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


std::string DECK = "J23456789TQKA";
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
        // pretend there are count(J) more of each card
        for (auto& card_count: card_counts) {
            if (card_count.first != 'J') {
                card_count.second += card_counts['J'];
            }
        }
        
        std::unordered_map<int, int> xok_counts;
        for (auto& card_count: card_counts) {
            if (card_count.first != 'J') {
                ++xok_counts[card_count.second];
            }
        }
        // 5 Js edge case
        if (xok_counts.empty()) {
            xok_counts[5] = 1;
        }
        
        // change conditions to account for double-counting with Js
        if (xok_counts[5] == 1) {
            // no ambiguity
            rank = HAND_RANKS["5ok"];
        } else if (xok_counts[4] >= 1) {
            // abJJJ -> 2x 4ok
            // aabJJ -> 1x 4ok
            // aaabJ -> 1x 4ok
            rank = HAND_RANKS["4ok"];
        } else if ((xok_counts[3] == 1 && xok_counts[2] == 1 && card_counts['J'] == 0)
                || (card_counts['J'] == 1 && xok_counts[3] == 2)) {
            // aabbJ -> 2x 3ok
            // aabcJ -> fake fh (not accepted)
            // aaabJ -> 4ok (not accepted)
            // aabJJ -> 4ok (not accepted)
            rank = HAND_RANKS["fh"];
        } else if (xok_counts[3] >= 1) {
            // abcJJ -> 3x 3ok
            // aabcJ -> 1x 3ok
            rank = HAND_RANKS["3ok"];
        } else if (xok_counts[2] == 2) {
            // impossible with Js, would get 3ok+
            rank = HAND_RANKS["2p"];
        } else if (xok_counts[2] >= 1) {
            // abcdJ -> 4x 2ok
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
    for (auto& hand : hands) std::cout << hand.cards << " " << hand.bid << " " << hand.rank << "\n";
    std::cout << std::endl;

    for (int i=0; i<hands.size(); ++i) {
        ans += (i+1)*hands[i].bid;
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}