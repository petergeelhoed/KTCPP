#include <cmath>
#include <concepts>
#include <iostream>
#include <sstream>
#include <string>

template <class T>
concept Ostreamable = requires(std::ostream& os, const T& arg)
{
    // clang-format off
    { os << arg } -> std::same_as<std::ostream&>;
    // clang-format on
};

class Concat
{
  public:
    explicit Concat(std::string sep) : m_sep(std::move(sep)) {}

    // Variadic template
    template <Ostreamable First, Ostreamable... Rest>
    std::string operator()(const First& first, const Rest&... rest) const
    {
        std::ostringstream oss;
        oss << first;
        // fold expression, repeat for all the rest
        ((oss << m_sep << rest), ...);
        oss << '\n';
        return oss.str();
    }

  private:
    std::string m_sep;
};

int main()
{
    const Concat c0(" ");
    const Concat c1(": ");
    std::cout << c0("C++");
    std::cout << c0("Peter", "François", "Geelhoed");
    std::cout << c1("Hans", 42);
    std::cout << c1("π", M_PI);
}
