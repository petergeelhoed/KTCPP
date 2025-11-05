#include <iostream>
#include <string>

// Base class

class Animal
{
  public:
    explicit Animal(const char* name) : m_name(name) {}

    Animal(const Animal&) = default;
    Animal& operator=(const Animal&) = default;
    Animal(Animal&&) = default;
    Animal& operator=(Animal&&) = default;

    virtual void makeSound() const = 0;
    virtual ~Animal() = default;

    void setName(const char* name) { m_name = name; }

    [[nodiscard]] std::string getName() const { return m_name; }

  private:
    std::string m_name;
};

class Cat : public Animal
{
  public:
    explicit Cat(const char* name) : Animal(name) {}

    void makeSound() const override
    {
        std::cout << getName() << " says miauw" << '\n';
    }
};

class Dog : public Animal
{
  public:
    explicit Dog(const char* name) : Animal(name) {}

    void makeSound() const override
    {
        std::cout << getName() << " says woof" << '\n';
    }
};

int main()
{
    Cat Tom("Tom");
    Dog Spike("Spike");
    Tom.makeSound();
    Spike.makeSound();

    Animal* animal = &Spike;
    Cat* Garfield = static_cast<Cat*>(animal);
    Garfield->setName("Garfield");
    Garfield->makeSound();

    Cat& Sylvester = static_cast<Cat&>(static_cast<Animal&>(Spike));
    Sylvester.setName("Sylvester");
    Sylvester.makeSound();

    return 0;
}
