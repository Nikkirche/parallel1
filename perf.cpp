#include <algorithm>
#include <iostream>
#include <benchmark/benchmark.h>

#include "quick_sort.h"
static int size = 100000000;
static std::mt19937 gen1{};

static void sequential(benchmark::State &state) {
    std::vector<int> v(size);
    std::iota(v.begin(), v.end(), 0);
    auto sp = std::span(v);
    for (auto _: state) {
        state.PauseTiming();
        std::ranges::shuffle(v, gen1);
        state.ResumeTiming();
        quick_sort_seq(sp);
    }
}

static void parallel(benchmark::State &state) {
    std::vector<int> v(size);
    std::vector<int> buff(size);
    std::iota(v.begin(), v.end(), 0);
    auto sp = std::span(v);
    auto sp_buff = std::span(buff);
    for (auto _: state) {
        state.PauseTiming();
        std::ranges::shuffle(v, gen1);
        state.ResumeTiming();
        quick_sort_par(sp, sp_buff, v.size() / state.range(0));
    }
}

static void parallel_without_copy(benchmark::State &state) {
    std::vector<int> v(size);
    std::iota(v.begin(), v.end(), 0);
    auto sp = std::span(v);
    for (auto _: state) {
        state.PauseTiming();
        std::ranges::shuffle(v, gen1);
        state.ResumeTiming();
        quick_sort_impl_par_without_copy(sp, v.size() / state.range(0));
    }
}

BENCHMARK(sequential)->Iterations(5)->MeasureProcessCPUTime()->UseRealTime();

BENCHMARK(parallel)->RangeMultiplier(2)->Range(4, 4 << 3)->Iterations(5)->MeasureProcessCPUTime()->UseRealTime();

BENCHMARK(parallel_without_copy)->RangeMultiplier(10)->Range(100, 100 * 1000)->Iterations(5)->MeasureProcessCPUTime()->
UseRealTime();
