#include <iostream>

template <unsigned int N, typename T> constexpr T pow_n(T number) noexcept
{
    if constexpr (N == 0)
    {
        return T{1};
    }
    else if constexpr (N == 1)
    {
        return number;
    }
    else
    {
        // N >= 2
        T result = T{1};
        T base = number;
        unsigned int exponent = N;
        while (exponent > 0)
        {
            if (exponent & 1U)
            {
                result *= base;
            }
            base *= base;
            exponent >>= 1U;
        }
        return result;
    }
}

template <typename T> constexpr T square(T number) noexcept
{
    return pow_n<2>(number);
}

int main()
{
    const double pi = 3.14159265358979323846264338327950;

    std::cout << pow_n<3>(3) << '\n';
    std::cout << pow_n<3>(pi) << '\n';

    std::cout << square(3) << '\n';
    std::cout << square(pi) << '\n';
}
