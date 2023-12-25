#include <string>
#include <vector>

namespace Util {
    template <typename T>
    std::string join(const T& collection, const std::string& delimiter);
    
    std::vector<std::string> split(const std::string& str, const std::string& delimiter);
}