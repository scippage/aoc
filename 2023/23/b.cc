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
#include <memory>
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

typedef std::vector<std::string> Trail;
typedef std::pair<int, int> Coord;
typedef Coord Vertex;
typedef std::tuple<Coord, Coord, int> Edge;
typedef std::map<Vertex, std::set<Edge>> Graph;

typedef std::tuple<std::set<Vertex>, Vertex, int> BacktrackState;
typedef std::tuple<int, Coord, Coord> QueueElm;
typedef std::priority_queue<QueueElm, std::vector<QueueElm>, std::greater<QueueElm>> Queue;

std::vector<Coord> neighbors(Trail& t, Coord& here, Coord& from) {
    std::vector<Coord> res;
    std::vector<Coord> candidates;
    auto [i, j] = here;
    candidates.push_back(std::make_pair(i, j+1));
    candidates.push_back(std::make_pair(i, j-1));
    candidates.push_back(std::make_pair(i-1, j));
    candidates.push_back(std::make_pair(i+1, j));
    for (auto candidate: candidates) {
        auto [c_i, c_j] = candidate;
        if (c_i >= 0 && c_i < t.size() && c_j >= 0 && c_j < t[0].size()) {
            if (t[c_i][c_j] != '#' && candidate != from) {
                res.push_back(candidate);
            }
        }
    }
    return res;
}

bool is_vertex(Trail& t, Coord& here) {
    auto [i, j] = here;
    int neighboring_slopes = 0;
    std::vector<Coord> candidates;
    candidates.push_back(std::make_pair(i, j+1));
    candidates.push_back(std::make_pair(i, j-1));
    candidates.push_back(std::make_pair(i-1, j));
    candidates.push_back(std::make_pair(i+1, j));
    for (auto candidate: candidates) {
        auto [c_i, c_j] = candidate;
        if (c_i >= 0 && c_i < t.size() && c_j >= 0 && c_j < t[0].size()) {
            auto tile = t[c_i][c_j];
            std::vector<char> slopes = {'<', '>', '^', 'v'};
            if (std::any_of(slopes.cbegin(), slopes.cend(), [tile](char c){ return tile == c; }))
                ++neighboring_slopes;
        }
    }
    return (neighboring_slopes > 1);
}

std::set<Vertex> find_vertices(Trail& t) {
    std::set<Vertex> res;
    for (int i=0; i<t.size(); ++i) {
        for (int j=0; j<t[0].size(); ++j) {
            if (t[i][j] == '#') continue;
            Coord coord = std::make_pair(i, j);
            if (is_vertex(t, coord)) res.insert(coord);
        }
    }
    return res;
}

std::set<Edge> find_edges(Trail& t, std::set<Vertex> vertices) {
    std::set<Edge> res;
    for (auto u: vertices) {
        for (auto n: neighbors(t, u, u)) {
            Coord from = u;
            Coord v = n;
            int weight = 1;
            while (!vertices.contains(v)) {
                // we know we are in a hallway, so there is only one valid neighbor
                auto ns = neighbors(t, v, from);
                from = v;
                v = ns[0];
                ++weight;
            }
            Edge e = std::make_tuple(u, v, weight);
            res.insert(e);
        }
    }
    return res;
}

Graph make_graph(std::set<Edge>& edges) {
    Graph g;
    for (auto e: edges) {
        auto [u, v, weight] = e;
        g[u].insert(e);
    }
    return g;
}

void print_graph(Graph& g) {
    // python graphviz format
    for (auto [vertex, edge_map] : g) {
        for (auto [u, v, weight]: edge_map) {
            printf("f.edge('(%d,%d)', '(%d,%d)', label='%d')\n", u.first, u.second, v.first, v.second, weight);
        }
    }
}

int backtrack(Graph& g, std::deque<BacktrackState>& state_queue, Vertex end) {
    // runs oom
    int max_cost = 0;
    while (!state_queue.empty()) {
        auto [visited, u, cost] = state_queue.front();
        state_queue.pop_front();
        if (u == end) {
            max_cost = std::max(max_cost, cost);
            // std::cout << max_cost << std::endl;
            continue;
        }
        if (visited.contains(u)) continue;
        visited.insert(u);
        for (auto [u, v, weight]: g[u]) {
            if (!visited.contains(v)) {
                BacktrackState new_state = std::make_tuple(visited, v, cost+weight);
                state_queue.push_back(new_state);
            }
        }
    }
    return max_cost;
}

int backtrack_rec(Graph& g, std::set<Vertex>& visited, Vertex u, int cost, Vertex end) {
    if (u == end) return cost;
    if (visited.contains(u)) return 0;

    int max_delta_cost = 0;
    for (auto [u, v, weight]: g[u]) {
        if (!visited.contains(v)) {
            visited.insert(u);
            max_delta_cost = std::max(max_delta_cost, backtrack_rec(g, visited, v, weight, end));
            visited.erase(u);
        }
    }
    if (max_delta_cost == 0) return 0;
    return cost + max_delta_cost;
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    Trail t;
    while (std::getline(file, line)) {
        t.push_back(line);
    }
    Coord start = std::make_pair(0, 1);
    Coord end = std::make_pair(t.size()-1, t[0].size()-2);

    // we observe that the "true" graph isn't that big because there are only a
    // small number of intersections connected by long hallways.
    // we can run a brute-force backtracking algorithm on this graph.
    auto vertices = find_vertices(t);
    vertices.insert(start);
    vertices.insert(end);
    std::cout << "|V|: " << vertices.size() << std::endl;
    auto edges = find_edges(t, vertices);
    std::cout << "|E|: " << edges.size() << std::endl;
    Graph g = make_graph(edges);
    // print_graph(g);

    std::deque<BacktrackState> state_queue;
    std::set<Vertex> visited;
    // BacktrackState init_state = std::make_tuple(visited, start, 0);
    // state_queue.push_back(init_state);
    // ans = backtrack(g, state_queue, end);
    ans = backtrack_rec(g, visited, start, 0, end);

    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}