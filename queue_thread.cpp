#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

constexpr size_t CACHE_SIZE = 64;
constexpr size_t SPIN_SIZE = 100;
constexpr size_t BUF_SIZE = 1024;

// NOLINTBEGIN[clang-analyzer-optin.performance.Padding] (I miss cache lines on
// purpose)
template <typename T, std::size_t Size> class RingBuffer
{
  public:
    RingBuffer() : buffer(Size) {}

    bool push(const T& value)
    {
        // safe because only producer pushes
        const auto head = m_head.load(std::memory_order_relaxed);
        const auto next = increment(head);

        // wait for buffer if m_tail is updated
        if (next == m_tail.load(std::memory_order_acquire))
        {
            return false; // buffer full
        }

        // need to make sure these instructions are not swapped
        buffer[head] = value;
        m_head.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& value)
    {
        const auto tail = m_tail.load(std::memory_order_relaxed);

        if (tail == m_head.load(std::memory_order_acquire))
        {
            return false; // buffer empty
        }

        value = buffer[tail];
        m_tail.store(increment(tail), std::memory_order_release);
        return true;
    }

  private:
    [[nodiscard]] std::size_t increment(std::size_t i) const
    {
        return (i + 1) % Size;
    }

    std::vector<T> buffer;

    alignas(CACHE_SIZE) std::atomic<std::size_t> m_head{0};
    alignas(CACHE_SIZE) std::atomic<std::size_t> m_tail{0};
};

// NOLINTEND[clang-analyzer-optin.performance.Padding]

void producer_rb(RingBuffer<int, BUF_SIZE>& rb, int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        size_t spins = 0;
        while (!rb.push(i))
        {
            if (spins < SPIN_SIZE)
            {
                ++spins;
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
}

void consumer_rb(RingBuffer<int, BUF_SIZE>& rb, int iterations, bool print)
{
    long long sum = 0;

    for (int i = 0; i < iterations; ++i)
    {
        size_t spins = 0;
        int value = 0;
        while (!rb.pop(value))
        {
            if (spins < SPIN_SIZE)
            {
                ++spins;
            }
            else
            {
                std::this_thread::yield();
            }
        }

        sum += value;
        if (print)
        {
            std::cout << "popped: " << value << ", sum so far: " << sum << '\n';
        }
    }
}

void producer(std::queue<int>& q,
              std::mutex& m,
              std::condition_variable& cv,
              int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        {
            const std::lock_guard<std::mutex> lock(m);
            q.push(i);
        }
        cv.notify_one();
    }
}

void consumer(std::queue<int>& q,
              std::mutex& m,
              std::condition_variable& cv,
              int iterations,
              bool print)
{
    long long sum = 0;

    for (int i = 0; i < iterations; ++i)
    {
        int value = 0;
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&] { return !q.empty(); });
            value = q.front();
            q.pop();
        }

        sum += value;
        if (print)
        {
            std::cout << "popped: " << value << ", sum so far: " << sum << '\n';
        }
    }
}

int main(int argc, [[maybe_unused]] char* argv[])
{
    constexpr int MILLION = 1000000;
    int iterations = MILLION;

    bool print = false;
    if ((print = (argc > 1)))
    {

        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,cert-err34-c)
        iterations = atoi(argv[1]);
        if (iterations == 0)
        {
            exit(-1);
        }
    }

    RingBuffer<int, BUF_SIZE> rb;
    std::queue<int> q;
    std::mutex m;
    std::condition_variable cv;

    auto start = std::chrono::high_resolution_clock::now();
    { // scoped for timing
        const std::jthread t1(
            producer, std::ref(q), std::ref(m), std::ref(cv), iterations);

        const std::jthread t2(consumer,
                              std::ref(q),
                              std::ref(m),
                              std::ref(cv),
                              iterations,
                              print);
    }
    auto mid = std::chrono::high_resolution_clock::now();
    { // scoped for timing
        const std::jthread t1(producer_rb, std::ref(rb), iterations);
        const std::jthread t2(consumer_rb, std::ref(rb), iterations, print);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "locks      Time: "
              << std::chrono::duration<double>(mid - start).count()
              << " seconds\n";
    std::cout << "ringbuffer Time: "
              << std::chrono::duration<double>(end - mid).count()
              << " seconds\n";
}

/*
1         locks  Time:  0.000843422  seconds  ringbuffer  Time:  0.000333484
seconds 4         locks  Time:  0.00082358   seconds  ringbuffer  Time:
0.000319004  seconds 16        locks  Time:  0.00102462   seconds  ringbuffer
Time:  0.00031239   seconds 64        locks  Time:  0.000593635  seconds
ringbuffer  Time:  0.000238902  seconds 256       locks  Time:  0.000900765
seconds  ringbuffer  Time:  0.000290776  seconds 1024      locks  Time:
0.00245662   seconds  ringbuffer  Time:  0.000494679  seconds 4096      locks
Time:  0.0089238    seconds  ringbuffer  Time:  0.00138696   seconds 16384 locks
Time:  0.0339568    seconds  ringbuffer  Time:  0.00450758   seconds 65536 locks
Time:  0.0995391    seconds  ringbuffer  Time:  0.0132895    seconds 262144
locks  Time:  0.39003      seconds  ringbuffer  Time:  0.0528479    seconds
1048576   locks  Time:  1.5713       seconds  ringbuffer  Time:  0.210548
seconds 4194304   locks  Time:  6.29187      seconds  ringbuffer  Time:  0.84374
seconds 16777216  locks  Time:  24.8634      seconds  ringbuffer  Time:  3.37518
seconds
*/
