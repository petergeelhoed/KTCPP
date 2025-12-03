#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace
{

struct Fibonacci
{
    unsigned int f1 = 0;
    unsigned int f2 = 1;

    unsigned int operator()()
    {
        auto result = f1;
        f1 = f2;
        f2 += result;
        return result;
    }
};

void fib_range(unsigned int beginrange, unsigned int endrange)
{
    if (endrange < beginrange)
    {
        throw std::invalid_argument("End should be >= begin");
    }
    std::vector<unsigned int> output(endrange + 1);

    // this took some research, but a stateful functor seems to be the way
    std::ranges::generate(output, Fibonacci{});

    for (auto f : output | std::views::drop(beginrange))
    {
        std::cout << f << '\n';
    }
}

void fib_mut_range(unsigned int beginrange, unsigned int endrange)
{
    if (endrange < beginrange)
    {
        throw std::invalid_argument("End should be >= begin");
    }
    std::vector<unsigned int> output;
    output.reserve(endrange + 1);

    auto fibs =
        std::views::iota(0U, endrange + 1) |
        std::views::transform(
            [f1 = 0U, f2 = 1U]([[maybe_unused]] unsigned int i) mutable {
                auto result = f1;
                f1 = f2;
                f2 += result;
                return result;
            });

    // dropping the beginning does not work because we need to go through the
    // process (lazy)
    for (auto f : fibs) //| std::views::drop(beginrange))
    {
        output.push_back(f);
    }
    for (auto f : output | std::views::drop(beginrange))
    {
        std::cout << f << '\n';
    }
}

unsigned int fib(unsigned int n) // NOLINT[misc-no-recursion]()
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
    if (endrange < beginrange)
    {
        throw std::invalid_argument("End should be >= begin");
    }

    auto r = std::views::iota(beginrange, endrange + 1) |
             std::views::transform([](unsigned int i) { return fib(i); });
    for (auto f : r)
    {
        std::cout << f << '\n';
    }
    std::cout << '\n';

    fib_mut_range(beginrange, endrange);
    std::cout << '\n';
    fib_range(beginrange, endrange);
}
