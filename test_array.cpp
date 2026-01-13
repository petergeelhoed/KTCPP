#include "array.hpp"

#include <iostream>
#include <string>

int main(void)
{
    Array<int, 3> ai{};
    ai[0] = 3;
    ai[1] = 2;
    ai[2] = 3;

    std::cout << "size: " << ai.size() << "\n";
    std::cout << "front: " << ai.front() << "\n";

    Array<const char*, 2> words{};
    words[0] = "hello";
    words[1] = "world";
    std::cout << words.front() << " " << words[1] << "\n";

    Array<std::string, 2> swords{};
    swords[0] = "hello";
    swords[1] = "world";
    std::cout << swords.front() << " " << swords[1] << "\n";
}
