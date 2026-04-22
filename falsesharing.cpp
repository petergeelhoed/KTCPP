#include <chrono>
#include <iostream>
#include <thread>

struct Counters
{
    volatile long long a = 0;
    volatile long long b = 0;
};

Counters counters;

void countA()
{
    for (long long i = 0; i < 500'000'000; ++i)
    {
        counters.a++;
    }
}

void countB()
{
    for (long long i = 0; i < 500'000'000; ++i)
    {
        counters.b++;
    }
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::thread t1(countA);
    std::thread t2(countB);

    t1.join();
    t2.join();

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "a = " << counters.a << '\n';
    std::cout << "b = " << counters.b << '\n';
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count()
              << " seconds\n";
}
