#include <sstream>
#include <string>
#include <vector>
#include "util.h"

namespace Util {
    template <typename T>
    std::string join(const T& collection, const std::string& delimiter) {
        std::ostringstream oss;
        auto begin = std::begin(collection);
        auto end = std::end(collection);
        if (begin != end) {
            oss << *begin++;
            for (; begin != end; ++begin)
                oss << delimiter << *begin;
        }
        return oss.str();
    }
    template std::string join(const std::vector<std::string>& collection, const std::string& delimiter);

    std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> vec;
        size_t last_idx = 0;
        size_t next_idx = 0;
        while ((next_idx = str.find(delimiter, last_idx)) != std::string::npos) {
            vec.push_back(str.substr(last_idx, next_idx-last_idx));
            last_idx = next_idx + delimiter.size();
        }
        vec.push_back(str.substr(last_idx));
        return vec;
    }
}