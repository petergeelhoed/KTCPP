

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

} // namespace math

int main()
{
    const math::Complex c1{.re = 1.0, .im = 2.0};
    const math::Complex c2{.re = 3.0, .im = 4.0};

    // works because of ADL
    [[maybe_unused]] auto c3 = c1 + c2;
}
