#include <iostream>
#include <memory>

// =============================
// Base class
// =============================
class Base
{
  public:
    Base() = default;
    virtual ~Base() = default;

    // Prevent slicing
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

    // Polymorphic clone
    [[nodiscard]] virtual std::unique_ptr<Base> clone() const = 0;

    virtual void speak() const = 0;
};

class Derived : public Base
{
  public:
    explicit Derived(int v) : m_val(v) {}

    ~Derived() override = default;

    // Custom copy constructor:
    //  - does NOT try to copy Base<Derived>
    //  - copies only Derived members
    Derived(const Derived& other) : m_val(other.m_val) {}

    Derived(Derived&& other) noexcept : m_val(other.m_val) {}

    Derived& operator=(const Derived& other)
    {
        if (this != &other)
        {
            m_val = other.m_val;
        }
        return *this;
    }

    Derived& operator=(Derived&& other) noexcept
    {
        if (this != &other)
        {
            m_val = other.m_val;
        }
        return *this;
    }

    [[nodiscard]] std::unique_ptr<Base> clone() const override
    {
        return std::make_unique<Derived>(*this);
    }

    void speak() const override
    {
        std::cout << "Derived says m_val = " << m_val << "\n";
    }

  private:
    int m_val;
};

int main()
{
    const std::unique_ptr<Base> b = std::make_unique<Derived>(3141593);

    b->speak();

    auto copy = b->clone();
    copy->speak();

    // Prevented slicing:
    // Base b2 = Derived(10); // illegal

    return 0;
}
