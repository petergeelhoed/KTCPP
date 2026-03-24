#include <iostream>
#include <memory>

// =============================
// Base class
// =============================
template <class DerivedT> class Base
{
  public:
    Base() = default;
    virtual ~Base() = default;

    // Prevent slicing by disabling copy/move through Base
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

    // Polymorphic clone via CRTP
    [[nodiscard]] std::unique_ptr<DerivedT> clone() const
    {
        return std::make_unique<DerivedT>(derived());
    }

  protected:
    // Access derived class
    [[nodiscard]] const DerivedT& derived() const noexcept
    {
        return static_cast<const DerivedT&>(*this);
    }

    [[nodiscard]] DerivedT& derived() noexcept
    {
        return static_cast<DerivedT&>(*this);
    }
};

class Derived : public Base<Derived>
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

    void speak() const
    {
        std::cout << "Derived says m_val = " << m_val << "\n";
    }

  private:
    int m_val;
};

int main()
{
    const Derived d1(3141593);
    d1.speak();

    // Safe clone, no slicing:
    auto d2 = d1.clone();
    d2->speak();

    Derived dCopy(*d2);
    dCopy.speak();

    Derived dAssign(2718282);
    dAssign.speak();
    dAssign = dCopy;
    dAssign.speak();

    Derived dMoved(std::move(dCopy));
    dMoved.speak();

    Derived dMovedAssign(1618034);
    dMovedAssign.speak();
    dMovedAssign = std::move(dMoved);
    dMovedAssign.speak();

    return 0;
}
