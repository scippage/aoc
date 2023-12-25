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
            return conjunction_state_sum == 0;
        }
        return true;
    }
};

void push_button(std::map<module_id, Module>& modules, int64_t& hi_ct, int64_t& lo_ct) {
    std::deque<std::pair<module_id, int>> queue;
    queue.push_back(std::make_pair(BROADCASTER, 0));
    lo_ct += 1; // button->broadcaster
    while (!queue.empty()) {
        auto [m_id, pulse] = queue.front();
        queue.pop_front();
        Module m = modules[m_id];
        for (auto& dest_id: m.destinations) {
            hi_ct += pulse;
            lo_ct += (1-pulse);
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

    int64_t hi_ct = 0;
    int64_t lo_ct = 0;
    int64_t cycle_len = 0;
    while (true) {
        ++cycle_len;
        push_button(modules, hi_ct, lo_ct);
        if (all_original_state(modules) || cycle_len==1000) break;
    }
    std::cout << "Cycle len: " << cycle_len << std::endl;

    hi_ct *= (1000 / cycle_len);
    lo_ct *= (1000 / cycle_len);
    int64_t remainder = 1000 % cycle_len;
    for (int64_t i=0; i<remainder; ++i) {
        push_button(modules, hi_ct, lo_ct);
    }

    std::cout << "Hi: " << hi_ct << ", Lo: " << lo_ct << std::endl;
    ans = hi_ct*lo_ct;
    std::cout << ans << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Run in " << runtime.count() << "ms" << std::endl;
    exit(EXIT_SUCCESS);
}