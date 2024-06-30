#include <benchmark/benchmark.h>
#include "src/utils/thread_safe_queue.h"
#include "test/common/boost_queue_adapter.h"

using namespace Util;


template <template<typename> class QueueType>
static void BM_PushPop(benchmark::State& state) {
    QueueType<int> queue;
    for (auto _ : state) {
        state.PauseTiming();
        const int NUM_OPERATIONS = state.range(0);
        state.ResumeTiming();
        
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            queue.Push(i);
        }
        for (int i = 0; i < NUM_OPERATIONS; ++i) {
            queue.Pop();
        }
    }
    state.SetItemsProcessed(state.iterations() * state.range(0) * 2);
}


// Benchmark for Push only
template <template<typename> class QueueType>
static void BM_Push(benchmark::State& state) {
    QueueType<int> queue;
    for (auto _ : state) {
        queue.Push(42);
    }
    state.SetItemsProcessed(state.iterations());
}

// Benchmark for Pop only
template <template<typename> class QueueType>
static void BM_Pop(benchmark::State& state) {
    QueueType<int> queue;
    for (auto _ : state) {
        state.PauseTiming();
        queue.Push(42);
        state.ResumeTiming();
        queue.Pop();
    }
    state.SetItemsProcessed(state.iterations());
}

//BENCHMARK_TEMPLATE(BM_Pop, ThreadSafeQueue);
//BENCHMARK_TEMPLATE(BM_Push, ThreadSafeQueue);
BENCHMARK_TEMPLATE(BM_PushPop, ThreadSafeQueue)->Range(8, 8<<10);

//BENCHMARK_TEMPLATE(BM_Pop, BoostLockfreeQueue);
//BENCHMARK_TEMPLATE(BM_Push, BoostLockfreeQueue);
BENCHMARK_TEMPLATE(BM_PushPop, BoostLockfreeQueue)->Range(8, 8<<10);

// will hang
//BENCHMARK_TEMPLATE(BM_Push, BoostLockfreeSPSCQueue);
//BENCHMARK_TEMPLATE(BM_Pop, BoostLockfreeSPSCQueue);
BENCHMARK_TEMPLATE(BM_PushPop, BoostLockfreeSPSCQueue)->Range(8, 8<<10);
