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


std::pair<double, double> quadratic_roots(double a, double b, double c) {
    double x1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
    double x2 = (-b - sqrt(b*b - 4*a*c))/(2*a);
    return std::make_pair(x1, x2);
}

int main() {
    std::ifstream file("./input.txt");
    std::string line;
    int64_t ans = 1;

    std::getline(file, line);
    std::istringstream iss1(line.substr(sizeof("Time:")));
    std::vector<int64_t> times{std::istream_iterator<int64_t>(iss1), 
                                 std::istream_iterator<int64_t>()};
    std::getline(file, line);
    std::istringstream iss2(line.substr(sizeof("Distance:")));
    std::vector<int64_t> distances{std::istream_iterator<int64_t>(iss2), 
                                 std::istream_iterator<int64_t>()};

    for (auto& time : times) std::cout << time << " ";
    std::cout << std::endl;
    for (auto& distance : distances) std::cout << distance << " ";
    std::cout << std::endl;

    auto concat_nums = [](int64_t a, int64_t b){
        int digs = std::to_string(b).size();
        return a*(pow(10, digs)) + b;
    };
    int64_t time = std::accumulate(times.begin(), times.end(), (int64_t)0, concat_nums);
    int64_t distance = std::accumulate(distances.begin(), distances.end(), (int64_t)0, concat_nums);
    std::cout << time << std::endl;
    std::cout << distance << std::endl;

    int64_t t = time;
    int64_t d = distance;
    const double EPS = 1.0e-5;

    // to solve:
    // (t-x)x > d+eps
    // -x^2 + tx - d-eps > 0
    // concave, roots always positive
    // solution is the range between the roots if there are multiple
    auto roots = quadratic_roots(-1, t, -d-EPS);
    int64_t ways = floor(roots.second) - ceil(roots.first) + (int64_t)1;
    printf("r1:%f r2:%f\n", roots.first, roots.second);
    printf("t:%" PRId64 " d:%" PRId64 " x:%" PRId64 "\n", t, d, ways);
    ans *= ways;

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}