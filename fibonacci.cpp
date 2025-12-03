#include <iostream>
#include <ranges>

namespace
{
unsigned int fib(const unsigned int n) // NOLINT[misc-no-recursion]()
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return fib(n - 2) + fib(n - 1);
}
} // namespace

int main()
{
    const unsigned int beginrange = 5;
    const unsigned int endrange = 8;
    auto r = std::views::iota(beginrange, endrange + 1) |
             std::views::transform([](const unsigned int i) { return fib(i); });
    for (auto f : r)
    {
        std::cout << f << '\n';
    }
}
