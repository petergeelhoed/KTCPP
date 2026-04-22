#include <chrono>
#include <iostream>
#include <thread>

struct Counter
{
    long long value = 0;
};

Counter a;
Counter b;

void countA()
{
    for (long long i = 0; i < 500'000'000; ++i)
    {
        a.value++;
    }
}

void countB()
{
    for (long long i = 0; i < 500'000'000; ++i)
    {
        b.value++;
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

    std::cout << "a = " << a.value << '\n';
    std::cout << "b = " << b.value << '\n';
    std::cout << "Time: " << std::chrono::duration<double>(end - start).count()
              << " seconds\n";
}

/*
   Output on a Xeon, as apparently a raspberry pi is not affected.

saturnus:~/$ g++ falsesharing.cpp && a.out
a = 500000000
b = 500000000
Time: 1.06541 seconds
saturnus:~/$ vi falsesharing.cpp
saturnus:~/$ g++ falsesharing.cpp && a.out
a = 500000000
b = 500000000
Time: 2.1494 seconds
*/
