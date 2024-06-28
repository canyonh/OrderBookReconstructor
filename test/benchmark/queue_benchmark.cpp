#include <benchmark/benchmark.h>
#include "utils/thread_safe_queue.h"

using namespace Util;

static void BM_PushPop(benchmark::State& state) {
    ThreadSafeQueue<int> queue;
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

// Register the benchmark
BENCHMARK(BM_PushPop)->Range(8, 8<<10);

// Benchmark for Push only
static void BM_Push(benchmark::State& state) {
    ThreadSafeQueue<int> queue;
    for (auto _ : state) {
        queue.Push(42);
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Push);

// Benchmark for Pop only
static void BM_Pop(benchmark::State& state) {
    ThreadSafeQueue<int> queue;
    for (auto _ : state) {
        state.PauseTiming();
        queue.Push(42);
        state.ResumeTiming();
        queue.Pop();
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Pop);
