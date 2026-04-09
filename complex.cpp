
#include <iostream>
#include <utility>

template <typename T>
void myswap(T& a, T& b) noexcept(noexcept(std::swap(a, b)))
{
    using std::swap; // enable ADL
    swap(a, b);

    // if std::swap(a,b) then ADL would be disabled.
}

namespace math
{
struct Complex
{
    double re, im;
};

Complex operator+(const Complex& a, const Complex& b)
{
    return {a.re + b.re, a.im + b.im};
}

void print(Complex& a) { std::cout << "(" << a.re << " ," << a.im << ")\n"; }

// optional, because my struct is perfectly swappable by std::swap
// but hey this is the excercise
void swap(Complex& a, Complex& b) noexcept
{
    std::cout << "custom swap\n";
    using std::swap;
    swap(a.re, b.re);
    swap(a.im, b.im);
}

} // namespace math

int main()
{
    math::Complex c1{.re = 1.0, .im = 2.0};
    math::Complex c2{.re = 3.0, .im = 4.0};

    math::print(c1);
    myswap(c1, c2);
    math::print(c1);

    double pi = 3.1416;
    double euler = 2.7183;

    myswap(pi, euler);
    std::cout << "π ≠ " << pi << '\n';

    // works because of ADL
    [[maybe_unused]] auto c3 = c1 + c2;
}
