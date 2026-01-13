
#include <cstddef>

template <typename T, std::size_t N> class Array
{
  public:
    [[nodiscard]] constexpr std::size_t size() const noexcept { return N; }

    constexpr T& front() noexcept { return m_data[0]; }

    constexpr const T& front() const noexcept { return m_data[0]; }

    constexpr T& operator[](size_t i) noexcept { return m_data[i]; }

    constexpr const T& operator[](size_t i) const noexcept { return m_data[i]; }

  private:
    T m_data[N]{};
};
