
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

class istream_iter
{
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string;
    using difference_type = std::ptrdiff_t;

    istream_iter() noexcept = default;

    explicit istream_iter(std::istream& instr, char sep)
      : m_instr(&instr), m_sep(sep)
    {
        read_next();
    }

    const std::string& operator*() const noexcept { return m_token; }

    const std::string* operator->() const noexcept { return &m_token; }

    istream_iter& operator++()
    {
        read_next();
        return *this;
    }

    void operator++(int) { ++(*this); }

    friend bool operator==(const istream_iter& left,
                           const istream_iter& right) noexcept
    {
        return left.m_end && right.m_end;
    }

    friend bool operator!=(const istream_iter& left,
                           const istream_iter& right) noexcept
    {
        return !(left == right);
    }

    // Iterator-sentinel comparisons (needed for ranges algorithms)
    friend bool operator==(const istream_iter& iter,
                           std::default_sentinel_t /*sentinel*/) noexcept
    {
        return iter.m_end;
    }

    friend bool operator!=(const istream_iter& iter,
                           std::default_sentinel_t /*sentinel*/) noexcept
    {
        return !iter.m_end;
    }

  private:
    std::istream* m_instr = nullptr;
    std::string m_token;
    char m_sep = ';';
    bool m_end = true;

    void read_next()
    {
        if (m_instr == nullptr)
        {
            m_end = true;
            return;
        }
        m_token.clear();

        if (!std::getline(*m_instr, m_token, m_sep))
        {
            m_end = true;
            m_instr = nullptr;
            return;
        }
        m_end = false;
    }
};

int main()
{
    // get the sep from argv if you want.
    char sep = ';';

    istream_iter begin(std::cin, sep);
    std::vector<std::string> vec;

    // This only works with std::ranges
    std::default_sentinel_t end;
    std::ranges::copy(begin, end, std::back_inserter(vec));

    // But this works with std::copy
    // std::copy(begin, istream_iter{}, std::back_inserter(vec));

    for (const auto& str : vec)
    {
        std::cout << str << "\n";
    }

    // EX2
    // for csv we need to handle
    // - quoted fields
    // - newlines in fields
    // - quotes in fields
    // - separators in fields
    //
    // we would make another iterator to get an entire line at the
    // time, and parse that using the separator iterator
    // which would need to be extended to handle quotes:
    // if we find a " get text until the next one that isn't escaped.
    // then start looking for comma's again.
}
