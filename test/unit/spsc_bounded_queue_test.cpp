#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <gtest/gtest.h>

#include "src/utils/spsc_bounded_queue.h"

using namespace Util;

static constexpr size_t QUEUE_CAPACITY = 16; // Power of 2
                                             //
template <typename QueueType>
class SPSCBoundedQueueTest : public ::testing::Test {
protected:
    QueueType queue;
};

using QueueTypes = ::testing::Types<
    SPSCBoundedQueueSeqCst<int, QUEUE_CAPACITY>,
    SPSCBoundedQueueSeqCstFalseSharing<int, QUEUE_CAPACITY>,
    SPSCBoundedQueueModulous<int, QUEUE_CAPACITY>,
    SPSCBoundedQueue<int, QUEUE_CAPACITY>
>;

TYPED_TEST_SUITE(SPSCBoundedQueueTest, QueueTypes);

TYPED_TEST(SPSCBoundedQueueTest, PushPopBasic) {
    EXPECT_TRUE(this->queue.Push(1));
    EXPECT_TRUE(this->queue.Push(2));
    EXPECT_TRUE(this->queue.Push(3));

    auto elem = this->queue.Pop();
    EXPECT_TRUE(elem.has_value());
    EXPECT_EQ(*elem, 1);

    elem = this->queue.Pop();
    EXPECT_TRUE(elem.has_value());
    EXPECT_EQ(*elem, 2);

    elem = this->queue.Pop();
    EXPECT_TRUE(elem.has_value());
    EXPECT_EQ(*elem, 3);

    elem = this->queue.Pop();
    EXPECT_FALSE(elem.has_value());
}

TYPED_TEST(SPSCBoundedQueueTest, Capacity) {
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        EXPECT_TRUE(this->queue.Push(i));
    }
    EXPECT_FALSE(this->queue.Push(QUEUE_CAPACITY)); // Queue is full

    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        auto elem = this->queue.Pop();
        EXPECT_TRUE(elem.has_value());
        EXPECT_EQ(*elem, i);
    }
    EXPECT_FALSE(this->queue.Pop().has_value()); // Queue is empty
}

TYPED_TEST(SPSCBoundedQueueTest, MoveSemantics) {
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        EXPECT_TRUE(this->queue.Push(std::move(i)));
    }
    EXPECT_FALSE(this->queue.Push(QUEUE_CAPACITY)); // Queue is full

    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        auto elem = this->queue.Pop();
        EXPECT_TRUE(elem.has_value());
        EXPECT_EQ(*elem, i);
    }
    EXPECT_FALSE(this->queue.Pop().has_value()); // Queue is empty
}

TYPED_TEST(SPSCBoundedQueueTest, ConcurrentPushPop) {
    static constexpr int NUM_OPERATIONS = 10000;

    std::thread producer([&]() {
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            while (!this->queue.Push(i)) {
                std::this_thread::yield();
            }
        }
    });

    std::thread consumer([&]() {
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            std::optional<int> elem;
            while (!(elem = this->queue.Pop())) {
                std::this_thread::yield();
            }
            EXPECT_EQ(*elem, i);
        }
    });

    producer.join();
    consumer.join();
}

TYPED_TEST(SPSCBoundedQueueTest, CapacityPowerOfTwo) {
    EXPECT_EQ(QUEUE_CAPACITY & (QUEUE_CAPACITY - 1), 0);
}
