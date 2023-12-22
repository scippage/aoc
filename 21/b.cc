#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <climits>
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
#include <queue>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include "../util/util.h"


typedef std::string Row;
typedef std::vector<Row> Garden;
typedef std::pair<int, int> Coord;
typedef std::pair<int, Coord> QueueElm;
typedef std::priority_queue<QueueElm, std::vector<QueueElm>, std::greater<QueueElm>> Queue;

std::vector<Coord> neighbors(Garden& g, Coord& here) {
    std::vector<Coord> res;
    auto [i, j] = here;
    Coord candidates[] = {std::make_pair(i+1, j), std::make_pair(i-1, j), std::make_pair(i, j+1), std::make_pair(i, j-1)};
    for (auto candidate: candidates) {
        auto [c_i, c_j] = candidate;
        if (c_i >= 0 && c_i < g.size() && c_j >= 0 && c_j < g[0].size()) {
            if (g[c_i][c_j] == '.') {
                res.push_back(candidate);
            }
        }
    }
    return res;
}

void dijkstras(Garden& g, std::set<Coord>& visited, std::map<Coord, int>& cost, Queue& queue) {
    while (!queue.empty()) {
        auto [priority, coord] = queue.top();
        queue.pop();
        if (visited.contains(coord)) continue;
        visited.insert(coord);
        // printf("(%d, %d): %d\n", coord.first, coord.second, cost[coord]);

        for (auto& neighbor: neighbors(g, coord)) {
            if (!cost.contains(neighbor)) cost[neighbor] = INT_MAX;
            cost[neighbor] = std::min(cost[neighbor], cost[coord] + 1);
            queue.push(std::make_pair(cost[neighbor], neighbor));
        }
    }
}

std::vector<int> sorted_costs(std::map<Coord, int>& cost_map, int parity) {
    std::vector<int> costs;
    for (auto& [coord, c]: cost_map) {
        // we accept cells with the same parity as our budget
        if (c % 2 == parity) {
            costs.push_back(c);
        }
    }
    std::sort(costs.begin(), costs.end());
    return costs;
}

std::map<Coord, int> calc_costs(Garden& g, Coord& source) {
    std::set<Coord> visited;
    std::map<Coord, int> cost;
    Queue queue;
    queue.push(std::make_pair(0, source));
    cost[source] = 0;
    dijkstras(g, visited, cost, queue);
    return cost;
}

int64_t reach_given_budget(std::map<Coord, int>& cost_map, int64_t budget) {
    auto costs = sorted_costs(cost_map, budget % 2);
    auto it = std::upper_bound(costs.begin(), costs.end(), budget);
    return it-costs.begin();
}

int64_t add_diagonal_region(Garden& g, Coord region_entry, int64_t starting_budget) {
    if (starting_budget <= 0) return 0;
    int64_t Bp = starting_budget;
    int64_t n = g.size();
    int64_t blocks_max = (Bp+n-1)/n;  // ceil(Bp/n)
    int64_t blocks_remainder = Bp % n;
    // printf("bp: %" PRId64 ", blocks_max: %" PRId64 ", blocks_remainder: %" PRId64 "\n", Bp, blocks_max, blocks_remainder);
    auto costs = calc_costs(g, region_entry);
    int64_t delta = 0;
    // printf("Max possible reach: %" PRId64 "\n", reach_given_budget(costs, Bp));
    // printf("Reach given blocks_remainer+2*n: %" PRId64 "\n", reach_given_budget(costs, blocks_remainder+2*n));
    if (blocks_max > 2) delta += (blocks_max-2)*((blocks_max-2)+1)/2*reach_given_budget(costs, blocks_remainder+2*n);
    delta += (blocks_max-1)*reach_given_budget(costs, blocks_remainder+n);
    delta += blocks_max*reach_given_budget(costs, blocks_remainder);
    printf("Delta: %" PRId64 "\n", delta);
    return delta;
}

int64_t add_orthogonal_region(Garden& g, Coord region_entry, int64_t starting_budget) {
    if (starting_budget <= 0) return 0;
    int64_t Bp = starting_budget;
    int64_t n = g.size();
    int64_t blocks_max = (Bp+n-1)/n;  // ceil(Bp/n)
    int64_t blocks_remainder = Bp % n;
    // printf("bp: %" PRId64 ", blocks_max: %" PRId64 ", blocks_remainder: %" PRId64 "\n", Bp, blocks_max, blocks_remainder);
    auto costs = calc_costs(g, region_entry);
    int64_t delta = 0;
    // printf("Max possible reach: %" PRId64 "\n", reach_given_budget(costs, Bp));
    // printf("Reach given blocks_remainer+2*n: %" PRId64 "\n", reach_given_budget(costs, blocks_remainder+2*n));
    if (blocks_max > 2) delta += (blocks_max-2)*reach_given_budget(costs, blocks_remainder+2*n);
    if (blocks_max > 1) delta += reach_given_budget(costs, blocks_remainder+n);
    delta += reach_given_budget(costs, blocks_remainder);
    printf("Delta: %" PRId64 "\n", delta);
    return delta;
}


int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    // std::ifstream file("./input_sample.txt");
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    // int64_t BUDGET = 10;
    int64_t BUDGET = 26501365;
    
    Garden g;
    while (std::getline(file, line)) {
        g.push_back(line);
    }

    Coord start_coord;
    for (int i=0; i<g.size(); ++i) {
        for (int j=0; j<g[0].size(); ++j) {
            if (g[i][j] == 'S') {
                start_coord = std::make_pair(i, j);
                g[i][j] = '.';
            }
        }
    }
    printf("Start coord: (%d, %d)\n", start_coord.first, start_coord.second);

    for (int i=0; i<g.size(); ++i) {
        if (g[i].find("#") == std::string::npos) {
            std::cout << "Empty row " << i << std::endl;
        }
    }
    for (int j=0; j<g[0].size(); ++j) {
        std::string col_j = "";
        for (int i=0; i<g.size(); ++i) col_j += g[i][j];
        if (col_j.find("#") == std::string::npos) {
            std::cout << "Empty col " << j << std::endl;
        }
    }

    // our garden is dim (odd x odd). to avoid having to mess with phases, expand our tile into a 2x2 group of tiles
    // and treat that as our fundamental tile. that way we will always stay on the "black" checkerboard squares
    int n = g.size();
    for (int i=0; i<n; ++i) {
        g[i] = g[i] + g[i];
        g.push_back(g[i]);
    }
    n = g.size();
    printf("New tile shape: (%zd, %zd)\n", g.size(), g[0].size());
    
    auto cost = calc_costs(g, start_coord);
    auto s_to_tl = cost[std::make_pair(0, 0)];
    auto s_to_tr = cost[std::make_pair(0, n-1)];
    auto s_to_bl = cost[std::make_pair(n-1, 0)];
    auto s_to_br = cost[std::make_pair(n-1, n-1)];
    auto s_to_l = cost[std::make_pair(start_coord.first, 0)];
    auto s_to_r = cost[std::make_pair(start_coord.first, n-1)];
    auto s_to_u = cost[std::make_pair(0, start_coord.second)];
    auto s_to_d = cost[std::make_pair(n-1, start_coord.second)];
    printf("S to tl: %d, tr: %d, bl: %d, br: %d\n", s_to_tl, s_to_tr, s_to_bl, s_to_br);
    printf("S to l: %d, r: %d, u: %d, d: %d\n", s_to_l, s_to_r, s_to_u, s_to_d);

    auto s_cost = sorted_costs(cost, 0);
    int BLOCK_STEPS = s_cost.size();
    printf("Block steps: %d\n", BLOCK_STEPS);
    printf("Max budget reach: %" PRId64 "\n", reach_given_budget(cost, n*n));

    /*
         /6\
        /___\
       /|   |\
      /2| 5 |1\
     /__|___|__\
    /|  | H |  |\
    \|__|___|__|/
     \ 3|   |4 /
      \ |   | /
       \|___|/
        \   /
         \ /


    Denote our budget (step ct) by B.
    First note that our answer will be the number of tiles with cost s.t. cost%2==0 and cost<=B.
    Let tiles be size NxN.
    Entering a tile from any corner, we claim that we can maximally cover it with budget 2N since it's fairly sparse
        (we verify this in code). This 2N limit isn't necessary but makes it easier to describe the algorithm.

    We start in the tile containing H.
    To get to region 1, the shortest path can always go through the top right corner of H because each tile 
        has an empty ring along the border.
    Let B' be our remaining budget from the top right corner of H minus an additional 2 to enter the closest tile in region 1.

    Then we can reach ceil(B'/N) tiles in the leftmost column of region 1, but can only fully cover ceil(B'/N)-2 of them.
    We enter the two partially covered tiles from their bottom left corners with remaining budgets
        B'%N and (B'%N)+N respectively.
    
    If we wanted to reach the second column of region 1, we can fully cover ceil(B'/N)-3 of them, and can again partially
        cover two tiles with leftover budgets B'%N and (B'%N)+N.

    If we wanted to reach the ceil(B'/N)th column of region 1, we can only partially cover one tile with leftover
        budget B'%N.

    Let R(b) be the number of squares reachable with a tile when entering from the bottom left corner with remaining
        budget b.

    The total area of region 1 is thus sum_{1..ceil(B'/N)-2}R(2N) + ceil(B'/N)R(B'%N) + (ceil(B'/N)-1)R((B'%N)+N).

    For the combined region 5+6, note that our starting row/column is empty so the shortest distance to any tile
        is the straight path there. This is NOT the case for the sample input so this solution doesn't work there.

    Let B'' be our remaining budget starting from the closest tile above H, i.e. B'' := B-S_i-1.
    Let R'(b) be the number of squares reachable in a tile with budget b if we enter from the bottom 
        on T[N-1][S_j].

    The total area of region 5 is (ceil(B''/N)-2)R(2N).
    The total area of region 6 is R'(B''%N) + R'((B''%N)+N).

    We apply symmetry to the remaining regions and sum to get our answer.
    */
    
    int64_t B = BUDGET;
    int64_t Bp;
    
    // to top right
    Bp = B - s_to_tr - 2;
    std::cout << "\nAdding region TR" << std::endl;
    ans += add_diagonal_region(g, std::make_pair(n-1, 0), Bp);

    // to top left
    Bp = B - s_to_tl - 2;
    std::cout << "\nAdding region TL" << std::endl;
    ans += add_diagonal_region(g, std::make_pair(n-1, n-1), Bp);

    // to bot right
    Bp = B - s_to_br - 2;
    std::cout << "\nAdding region BR" << std::endl;
    ans += add_diagonal_region(g, std::make_pair(0, 0), Bp);

    // to bot left
    Bp = B - s_to_bl - 2;
    std::cout << "\nAdding region BL" << std::endl;
    ans += add_diagonal_region(g, std::make_pair(0, n-1), Bp);

    // to up
    std::cout << "\nAdding region U" << std::endl;
    Bp = B - s_to_u - 1;
    ans += add_orthogonal_region(g, std::make_pair(n-1, start_coord.second), Bp);

    // to down
    std::cout << "\nAdding region D" << std::endl;
    Bp = B - s_to_d - 1;
    ans += add_orthogonal_region(g, std::make_pair(0, start_coord.second), Bp);

    // to left
    std::cout << "\nAdding region L" << std::endl;
    Bp = B - s_to_l - 1;
    ans += add_orthogonal_region(g, std::make_pair(start_coord.first, n-1), Bp);
    
    // to right
    std::cout << "\nAdding region R" << std::endl;
    Bp = B - s_to_r - 1;
    ans += add_orthogonal_region(g, std::make_pair(start_coord.first, 0), Bp);

    // self
    std::cout << "\nAdding region SELF" << std::endl;
    int64_t delta = reach_given_budget(cost, B);
    printf("Delta: %" PRId64 "\n", delta);
    ans += delta;

    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}