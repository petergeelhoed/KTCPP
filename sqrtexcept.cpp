#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

double parseDouble(const std::string& s)
{
    size_t idx = 0;
    const double val = std::stod(s, &idx);

    while (idx < s.size() && std::isspace(static_cast<unsigned char>(s[idx])))
    {
        ++idx;
    }
    if (idx != s.size())
    {
        throw std::invalid_argument("Unexpected characters after the number.");
    }

    return val;
}

double safeSqrt(double x)
{
    if (std::isnan(x))
    {
        throw std::domain_error("Input is NaN.");
    }
    if (x < 0.0)
    {
        throw std::domain_error(
            "Square root of a negative number is undefined for real numbers.");
    }

    if (std::isinf(x))
    {
        throw std::overflow_error("Input is infinite.");
    }

    return std::sqrt(x);
}

int main()
{
    try
    {
        std::string line;
        if (!std::getline(std::cin, line))
        {
            throw std::runtime_error("Failed to read input.");
        }

        auto left = line.find_first_not_of(" \t\r\n");
        auto right = line.find_last_not_of(" \t\r\n");
        const std::string trimmed = (left == std::string::npos)
                                        ? std::string()
                                        : line.substr(left, right - left + 1);

        if (trimmed.empty())
        {
            throw std::invalid_argument("Empty input.");
        }

        const double value = parseDouble(trimmed);
        const double result = safeSqrt(value);

        const size_t precision = 12;
        std::cout.setf(std::ios::fixed);
        std::cout.precision(precision);
        std::cout << "sqrt(" << value << ") = " << result << "\n";
        return 0;
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Input error: " << e.what() << "\n";
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Range error: Input number is too large or too small. "
                  << e.what() << "\n";
    }
    catch (const std::domain_error& e)
    {
        std::cerr << "Domain error: " << e.what() << "\n";
    }
    catch (const std::overflow_error& e)
    {
        std::cerr << "Overflow error: " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << "\n";
    }
    return 1; // Non-zero exit code on error
}
