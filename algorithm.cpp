#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

void printVec(const std::vector<int>& ints)
{
    for (const auto& n : ints)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

void printMap(const std::unordered_map<int, std::string>& intMap)
{
    for (const auto& [key, value] : intMap)
    {
        std::cout << key << " -> ('" << value << "')\n";
    }
}

std::unordered_map<int, std::string> makeMap(const std::vector<int>& ints)
{
    std::unordered_map<int, std::string> m;
    std::transform(ints.begin(),
                   ints.end(),
                   std::inserter(m, m.end()),
                   [](int i) { return std::make_pair(i, std::to_string(i)); });
    return m;
}

int main()
{
    // added 7.1 and 5.4 to show != and == otherwist > and >= would also give
    // the right order
    std::cout << "original:\n";
    std::vector<int> ints = {1, 9, 4, 3, 5, 6, 7, 8, 2};
    printVec(ints);
    std::cout << "product: "
              << std::accumulate(
                     ints.begin(), ints.end(), 1, std::multiplies<int>())
              << "\n";

    std::cout << "sorted: "
              << "\n";
    std::sort(ints.begin(), ints.end());
    printVec(ints);

    std::cout << "map:\n";
    printMap(makeMap(ints));

    std::cout << "remove odd numbers\n";
    ints.erase(std::remove_if(ints.begin(),
                              ints.end(),
                              [](const int& n) { return n % 2 != 0; }),
               ints.end());
    printVec(ints);
}
