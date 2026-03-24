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

class Derived : private Base
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
        return std::unique_ptr<Base>(new Derived(*this));
    }

    void speak() const override
    {
        std::cout << "Derived says m_val = " << m_val << "\n";
    }

    // PUBLIC FACTORY: This is the ONLY way to create
    // a unique_ptr<Base> that owns a Derived.
    static std::unique_ptr<Base> create(int v)
    {
        return std::unique_ptr<Base>(new Derived(v));
    }

  private:
    int m_val;
};

int main()
{
    auto b = Derived::create(3141593);

    b->speak();

    auto copy = b->clone();
    copy->speak();

    // Prevented slicing:
    // Base b2 = Derived(10); // illegal

    return 0;
}
