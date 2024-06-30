#include <gtest/gtest.h>
#include "src/utils/thread_safe_queue.h"

namespace UnitTest
{

using namespace Util;

class ThreadSafeQueueTest : public testing::Test
{
};

TEST_F(ThreadSafeQueueTest, Basic)
{
    ThreadSafeQueue<int> queue;
    
    // Test Push and Pop
    queue.Push(1);
    auto result = queue.Pop();
    ASSERT_TRUE(result && *result == 1);
    
    // Test empty queue
    result = queue.Pop(std::chrono::milliseconds(10));
    ASSERT_TRUE(!result);
}

TEST_F(ThreadSafeQueueTest, MultiThreaded)
{
    ThreadSafeQueue<int> queue;
    std::atomic<int> sum(0);
    std::atomic<int> count(0);
    const int NUM_ITEMS = 100;
    const int NUM_THREADS = 4;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        producers.emplace_back([&queue, NUM_ITEMS] {
            for (int j = 0; j < NUM_ITEMS; ++j)
            {
                queue.Push(j);
            };
        });

        consumers.emplace_back([&queue, &sum, &count, NUM_ITEMS] {
            while (count < NUM_ITEMS * NUM_THREADS) {
                auto item = queue.Pop(std::chrono::milliseconds(50));
                if (item) {
                    sum += *item;
                    count++;
                }
            }
        });
    };

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    ASSERT_TRUE(count == NUM_ITEMS * NUM_THREADS);
    int expected_sum = (NUM_ITEMS - 1) * NUM_ITEMS / 2 * NUM_THREADS;
    ASSERT_TRUE(sum == expected_sum);
}

TEST_F(ThreadSafeQueueTest, Timeout)
{
    ThreadSafeQueue<int> queue;
    auto start = std::chrono::steady_clock::now();
    auto result = queue.Pop(std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();
    ASSERT_FALSE(result);
    ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), 100);
}

TEST_F(ThreadSafeQueueTest, Order)
{
    ThreadSafeQueue<int> queue;
    for (int i = 0; i < 100; ++i) {
        queue.Push(i);
    }
    for (int i = 0; i < 100; ++i) {
        auto result = queue.Pop();
        ASSERT_TRUE(result);
        ASSERT_EQ(*result, i);
    }
}

TEST_F(ThreadSafeQueueTest, MoveSemantics)
{
    ThreadSafeQueue<std::unique_ptr<int>> queue;
    queue.Push(std::make_unique<int>(42));
    auto result = queue.Pop();
    ASSERT_TRUE(result && **result == 42);
}

} // namespace UnitTest
