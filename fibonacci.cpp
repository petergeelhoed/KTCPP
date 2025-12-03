#include <iostream>
#include <ranges>

namespace
{
unsigned int fib_lin(unsigned int n)
{
    if (n < 2)
    {
        return n;
    }

    unsigned int f1 = 1;
    unsigned int f2 = 1;
    for (auto f : std::views::iota(2U, n) |
                      std::views::transform(
                          [&f1, &f2]([[maybe_unused]] const unsigned int i) {
                              return f1 + f2;
                          }))
    {
        f1 = f2;
        f2 = f;
    }
    return f2;
}

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
    std::cout << '\n';

    auto q =
        std::views::iota(beginrange, endrange + 1) |
        std::views::transform([](const unsigned int i) { return fib_lin(i); });
    for (auto f : q)
    {
        std::cout << f << '\n';
    }
}
