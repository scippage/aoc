#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
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
    int ans = 1;

    std::getline(file, line);
    std::istringstream iss1(line.substr(sizeof("Time:")));
    std::vector<int> times{std::istream_iterator<int>(iss1), 
                                 std::istream_iterator<int>()};

    std::getline(file, line);
    std::istringstream iss2(line.substr(sizeof("Distance:")));
    std::vector<int> distances{std::istream_iterator<int>(iss2), 
                                 std::istream_iterator<int>()};

    for (auto& time : times) std::cout << time << " ";
    std::cout << std::endl;
    for (auto& distance : distances) std::cout << distance << " ";
    std::cout << std::endl;

    for (int i=0; i<times.size(); i++) {
        int t = times[i];
        int d = distances[i];
        const double EPS = 1.0e-5;

        // to solve:
        // (t-x)x > d+eps
        // -x^2 + tx - d-eps > 0
        // concave, roots always positive
        // solution is the range between the roots if there are multiple
        auto roots = quadratic_roots(-1, t, -d-EPS);
        int ways = floor(roots.second) - ceil(roots.first) + 1;
        printf("r1:%f r2:%f\n", roots.first, roots.second);
        printf("t:%d d:%d x:%d\n", t, d, ways);
        ans *= ways;
    }

    std::cout << ans << std::endl;
    exit(EXIT_SUCCESS);
}