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


typedef std::string module_id;
typedef std::string module_type;
typedef std::pair<module_id, int> Pulse;
const module_id BROADCASTER = "broadcaster";

struct Module {
    module_id id;
    module_type type = "";
    std::vector<module_id> destinations;
    int flipflop_state = 0;
    std::map<module_id, int> conjunction_state;
    int conjunction_state_sum = 0;

    void init_module(std::string full_module_name) {
        if (full_module_name[0] == '&' || full_module_name[0] == '%') {
            type = full_module_name[0];
            id = full_module_name.substr(1);
        } else {
            id = full_module_name;
        }
    }

    std::optional<int> receive_pulse(module_id source, int pulse) {
        if (type == "%") {
            if (pulse == 1) return std::nullopt;
            else if (pulse == 0) {
                flipflop_state = flipflop_state^1;
                return flipflop_state;
            }
        }
        if (type == "&") {
            conjunction_state_sum += -conjunction_state[source] + pulse;
            conjunction_state[source] = pulse;
            if (conjunction_state_sum == conjunction_state.size()) {
                return 0;
            } else {
                return 1;
            }
        }
        return std::nullopt;
    }

    bool is_original_state() {
        if (type == "%") {
            return flipflop_state == 0;
        }
        if (type == "&") {
            // the second condition comes from inspecting our graph and finding
            // a commonality in the problematic nodes
            return conjunction_state_sum == 0 || destinations.size() == 1;
        }
        return true;
    }
};

void push_button(std::map<module_id, Module>& modules, Pulse& init_pulse) {
    std::deque<Pulse> queue;
    queue.push_back(init_pulse);
    while (!queue.empty()) {
        auto [m_id, pulse] = queue.front();
        queue.pop_front();
        Module m = modules[m_id];
        for (auto& dest_id: m.destinations) {
            auto feedback = modules[dest_id].receive_pulse(m.id, pulse);
            if (feedback.has_value()) {
                queue.push_back(std::make_pair(dest_id, feedback.value()));
            }
        }   
    }
}

bool all_original_state(std::map<module_id, Module>& modules) {
    for (auto& [m_id, m]: modules)
        if (!m.is_original_state())
            return false;
    return true;
}

void print_graph(std::map<module_id, Module>& modules) {
    // python graphviz format
    printf("import graphviz\n");
    printf("f = graphviz.Digraph('finite_state_machine', filename='fsm.gv')\n");
    printf("f.attr(rankdir='LR', size='8,5')\n");
    printf("f.attr('node', shape='doublecircle')\n");
    for (auto [module_id, m] : modules) {
        if (m.type == "&")
            printf("f.node('\\%s%s')\n", m.type.c_str(), module_id.c_str());
    }
    printf("f.attr('node', shape='circle')\n");
    for (auto [module_id, m] : modules) {
        for (auto d: m.destinations) {
            auto d_t = modules[d].type;
            printf("f.edge('\\%s%s', '\\%s%s')\n", m.type.c_str(), module_id.c_str(), d_t.c_str(), d.c_str());
        }
    }
    printf("f.view()\n");
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 0;
    
    std::map<module_id, Module> modules;
    while (std::getline(file, line)) {
        auto module_spec = Util::split(line, " -> ");
        Module m;
        m.init_module(module_spec[0]);
        m.destinations = Util::split(module_spec[1], ", ");
        modules[m.id] = m;
    }
    for (auto& [m_id, m] : modules) {
        // std::cout << m.type << m.id << "->" << Util::join(m.destinations, " ") << std::endl;
        for (auto& dest: m.destinations)
            if (modules[dest].type == "&")
                modules[dest].conjunction_state[m.id] = 0;
    }
    // print_graph(modules);

    /*
    We have to look at the graph for our specific input for this one.
    Our cycle detection code doesn't work in the general case because two singly-connected &->& nodes
        will always be out of sync.

    From inspection, broadcast sends to 4 different subgraphs with similar structure:
    broadcast -> a, b, c, d
    &a -> &a' -> &comb
    &b -> &b' -> &comb
    &c -> &c' -> &comb
    &d -> &d' -> &comb
    &comb -> dest

    Our cycle detection can detect the order A of the cycle from broadcast to &a, for instance.
    After lcm(A+B+C+D) presses, &comb will fire a low into dest.
    */

    // broadcast to only one connected component at a time
    std::vector<module_id> broadcast_dests = modules[BROADCASTER].destinations;
    std::vector<int> cycle_lengths;
    for (auto broadcast_dest: broadcast_dests) {
        modules[BROADCASTER].destinations = { broadcast_dest };
        int64_t cycle_len = 0;
        while (true) {
            ++cycle_len;
            Pulse init_pulse = std::make_pair(BROADCASTER, 0);
            push_button(modules, init_pulse);
            if (all_original_state(modules)) break;
        }
        cycle_lengths.push_back(cycle_len);
        std::cout << "Cycle len: " << cycle_len << std::endl;
    }

    ans = std::accumulate(cycle_lengths.begin(), cycle_lengths.end(), (int64_t)1, 
        [](int64_t a, int64_t b){ return std::lcm(a, b);}
    );
    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}