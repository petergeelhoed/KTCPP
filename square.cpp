

#include <iostream>

template <typename T> constexpr T square(T x) noexcept { return x * x; }

int main()
{
    const double pi = 3.14159265358979323846264338327950;
    std::cout << square(3) << '\n';
    std::cout << square(pi) << '\n';
}
