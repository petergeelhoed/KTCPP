#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <expected>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

enum class ErrorCode
{
    IoError,
    EmptyInput,
    InvalidNumber,
    UnexpectedChars,
    OutOfRange,
    DomainError,
    OverflowError
};

struct Error
{
    ErrorCode code;
    std::string message;
};

static inline std::string_view trim(std::string_view sv)
{
    const auto begin = sv.find_first_not_of(" \t\r\n");
    if (begin == std::string_view::npos)
    {
        return {};
    }
    const auto end = sv.find_last_not_of(" \t\r\n");
    return sv.substr(begin, end - begin + 1);
}

std::expected<double, Error> parseDouble(const std::string& s)
{
    const std::string_view t = trim(s);
    if (t.empty())
    {
        return std::unexpected(Error{ErrorCode::EmptyInput, "Empty input."});
    }

    errno = 0;
    std::string tmp(t);
    char* end = nullptr;
    const double val = std::strtod(tmp.c_str(), &end);

    if (end == tmp.c_str())
    {
        return std::unexpected(
            Error{ErrorCode::InvalidNumber, "Not a valid number."});
    }

    if (*end != '\0')
    {
        return std::unexpected(
            Error{ErrorCode::UnexpectedChars,
                  "Unexpected characters after the number."});
    }

    if (errno == ERANGE)
    {
        return std::unexpected(
            Error{ErrorCode::OutOfRange,
                  "Input number is too large or too small to represent."});
    }

    return val;
}

std::expected<double, Error> safeSqrt(double x)
{
    if (std::isnan(x))
    {
        return std::unexpected(Error{ErrorCode::DomainError, "Input is NaN."});
    }
    if (x < 0.0)
    {
        return std::unexpected(Error{
            ErrorCode::DomainError,
            "Square root of a negative number is undefined for real numbers."});
    }
    if (std::isinf(x))
    {
        return std::unexpected(
            Error{ErrorCode::OverflowError, "Input is infinite."});
    }

    const double y = std::sqrt(x);

    if (!std::isfinite(y))
    {
        return std::unexpected(
            Error{ErrorCode::OverflowError, "sqrt result is infinite."});
    }
    return y;
}

static inline const char* label_for(ErrorCode c)
{
    switch (c)
    {
    case ErrorCode::IoError:
        return "I/O error";
    case ErrorCode::EmptyInput:
        return "Input error";
    case ErrorCode::InvalidNumber:
        return "Input error";
    case ErrorCode::UnexpectedChars:
        return "Input error";
    case ErrorCode::OutOfRange:
        return "Range error";
    case ErrorCode::DomainError:
        return "Domain error";
    case ErrorCode::OverflowError:
        return "Overflow error";
    }
    return "Error";
}

int main()
{
    std::string line;
    if (!std::getline(std::cin, line))
    {
        std::cerr << label_for(ErrorCode::IoError)
                  << ": Failed to read input.\n";
        return 1;
    }

    auto parsed = parseDouble(line);
    if (!parsed)
    {
        std::cerr << label_for(parsed.error().code) << ": "
                  << parsed.error().message << "\n";
        return 1;
    }

    auto principal_root = safeSqrt(*parsed);
    if (!principal_root)
    {
        std::cerr << label_for(principal_root.error().code) << ": "
                  << principal_root.error().message << "\n";
        return 1;
    }

    constexpr int precision = 12;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(precision);
    std::cout << "sqrt(" << *parsed << ") = " << *principal_root << "\n";
    return 0;
}
