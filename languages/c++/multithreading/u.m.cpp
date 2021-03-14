#include "util.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>

struct Object
{
    int x;
    explicit operator std::size_t() const
    {
        return x+1;
    }
};

int main(int argc, char* argv[])
{
    std::cout << joinBy("-", 1, 1.15, "abc") << std::endl;

    std::vector<int> x = {1, 2, 3, 4, 5, 6, 1, 2, 3};


    std::set<int> y1;
    std::unordered_set<int> y2;

    std::copy(x.begin(), x.end(), std::inserter(y1, y1.end()));
    std::copy(x.begin(), x.end(), std::inserter(y2, y2.end()));

    std::cout << y1.size() << " " << y2.size() << std::endl;

    std::vector<std::string> z = {"1", "2.3" };
    std::vector<int> zint;
    std::transform(z.begin(), z.end(), std::back_inserter(zint), [](const std::string& s){ return std::stoi(s); });

    for (auto i : zint) { std::cout << i << '\n'; }

    return 0;
}
