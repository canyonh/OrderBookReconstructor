#include <benchmark/benchmark.h>
#include "src/utils/spsc_bounded_queue.h"
#include "src/utils/thread_safe_queue.h"
#include "test/common/boost_queue_adapter.h"

using namespace Util;

static constexpr size_t MIN_OPERATIONS = 128;
static constexpr size_t MAX_OPERATIONS = 256;

template <typename QueueType>
static void BM_PushPop(benchmark::State& state) {
    QueueType queue;
    for (auto _ : state) {
        state.PauseTiming();
        const int num_ops = state.range(0);
        state.ResumeTiming();

        std::thread producer([&]{
            for (int i = 0; i < num_ops; ++i) {
                bool success = false;
                while (!queue.Push(i)) {}
            }
        });
        
        std::thread consumer([&]{
            size_t cnt = 0;
            while (cnt < num_ops)
            {
                if (queue.Pop())
                    ++cnt;
            }
        });
        producer.join();
        consumer.join();
    }
    state.SetItemsProcessed(state.iterations() * state.range(0) * 2);
}

BENCHMARK_TEMPLATE(BM_PushPop, ThreadSafeQueue<int>)->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, BoostLockfreeQueue<int>)->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, BoostLockfreeSPSCQueue<int>)->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, SPSCBoundedQueueSeqCstFalseSharing<int, MAX_OPERATIONS> )->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, SPSCBoundedQueueSeqCst<int, MAX_OPERATIONS> )->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, SPSCBoundedQueueModulous<int, MAX_OPERATIONS> )->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, SPSCBoundedQueue<int, MAX_OPERATIONS> )->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, SPSCBoundedQueueUnlikely<int, MAX_OPERATIONS> )->Range(MIN_OPERATIONS, MAX_OPERATIONS);
BENCHMARK_TEMPLATE(BM_PushPop, SPSCBoundedQueueCachedSize<int, MAX_OPERATIONS> )->Range(MIN_OPERATIONS, MAX_OPERATIONS);
