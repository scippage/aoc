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
#include <Eigen/Dense>
#include "../util/util.h"

typedef std::string Node;
typedef std::pair<Node, Node> Edge;

void dfs(std::map<Node, std::set<Node>>& nodes, std::set<Edge>& edges, std::set<Node>& visited, Node curr) {
    if (visited.contains(curr)) return;
    visited.insert(curr);
    for (Node neighbor: nodes[curr]) {
        Edge e1 = std::make_pair(curr, neighbor);
        Edge e2 = std::make_pair(neighbor, curr);
        if (!edges.contains(e1) && !edges.contains(e2)) continue;
        if (!visited.contains(neighbor)) {
            dfs(nodes, edges, visited, neighbor);
        }
    }
}

void save_graph(std::set<Edge>& edges, std::string filename) {
    // python graphviz format
    std::ofstream file;
    file.open(filename);
    file << "import graphviz\n";
    file << "f = graphviz.Graph('finite_state_machine', filename='fsm.gv', engine='sfdp'))\n";
    file << "f.attr(rankdir='LR', size='80.5', dpi='3000'))\n";
    file << "f.attr('node', shape='circle')\n";
    for (auto e : edges) {
        file << "f.edge('" << e.first.c_str() << "', '" << e.second.c_str() << "')\n";
    }
    file << "f.view()\n";
    file.close();
}


int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::map<Node, std::set<Node>> nodes;
    std::set<Edge> edges;
    std::vector<Edge> edges_vec;
    while (std::getline(file, line)) {
        auto l_r = Util::split(line, ": ");
        auto ns = Util::split(l_r[1], " ");
        for (auto n: ns) {
            nodes[l_r[0]].insert(n);
            nodes[n].insert(l_r[0]);
            edges.insert(std::make_pair(l_r[0], n));
            edges_vec.push_back(std::make_pair(l_r[0], n));
        }
    }
    std::cout << "Nodes: " << nodes.size() << std::endl;
    std::cout << "Edges: " << edges.size() << std::endl;
    // save_graph(edges, "graphviz.txt");

    /*
        We cheated the problem using graphviz.
        The SDFP layout treats nodes as mutually repelling electons joined
        by springs where there are edges, so given the nature of the graph,
        the cut nodes will lay out close to each other and we can visually
        see what the cut edges must be.

        Algorithmically, we could've instead tried something like Edmonds-Karp.
    */
    int iter = 0;
    for (int i=0; i<edges_vec.size(); ++i) {
        for (int j=i+1; j<edges_vec.size(); ++j) {
            for (int k=j+1; k<edges_vec.size(); ++k) {
                ++iter;
                if (iter == 1) std::cout << iter << std::endl;
                if (iter == 100) std::cout << iter << std::endl;
                if (iter == 10000) std::cout << iter << std::endl;
                if (iter == 1000000) std::cout << iter << std::endl;
                if (iter == 100000000) std::cout << iter << std::endl;

                edges.erase(std::make_pair("jpn", "vgf"));
                edges.erase(std::make_pair("vgf", "jpn"));
                edges.erase(std::make_pair("mnl", "nmz"));
                edges.erase(std::make_pair("nmz", "mnl"));
                edges.erase(std::make_pair("txm", "fdb"));
                edges.erase(std::make_pair("fdb", "txm"));

                // edges.erase(edges_vec[i]);
                // edges.erase(edges_vec[j]);
                // edges.erase(edges_vec[k]);
                // printf("erase %s,%s\n", edges_vec[i].first.c_str(), edges_vec[i].second.c_str());
                // printf("erase %s,%s\n", edges_vec[j].first.c_str(), edges_vec[j].second.c_str());
                // printf("erase %s,%s\n", edges_vec[k].first.c_str(), edges_vec[k].second.c_str());
                // std::cout << "Edges: " << edges.size() << std::endl;

                Node curr;
                bool found_curr;
                for (auto [node, ns]: nodes) {
                    for (auto neighbor: ns) {
                        Edge e1 = std::make_pair(node, neighbor);
                        Edge e2 = std::make_pair(neighbor, node);
                        if (edges.contains(e1) || edges.contains(e2)) {
                            curr = node;
                            found_curr = true;
                            break;
                        }
                    }
                    if (found_curr) break;
                }
                if (!found_curr) throw std::domain_error("didn't find start node");
                found_curr = false;

                std::set<Node> visited;
                dfs(nodes, edges, visited, curr);
                if (visited.size() != nodes.size()) {
                    std::cout << "Visited: " << visited.size() << std::endl;
                    printf("erase %s,%s\n", edges_vec[i].first.c_str(), edges_vec[i].second.c_str());
                    printf("erase %s,%s\n", edges_vec[j].first.c_str(), edges_vec[j].second.c_str());
                    printf("erase %s,%s\n", edges_vec[k].first.c_str(), edges_vec[k].second.c_str());
                    ans = visited.size() * (nodes.size() - visited.size());
                    goto end;
                }

                edges.insert(edges_vec[i]);
                edges.insert(edges_vec[j]);
                edges.insert(edges_vec[k]);
            }
        }
    }
    end:


    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}