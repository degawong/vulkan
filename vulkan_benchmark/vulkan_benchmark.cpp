/*
 * @Author: your name
 * @Date: 2021-09-03 16:12:07
 * @LastEditTime: 2021-09-06 09:20:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_benchmark\vulkan_benchmark.cpp
 */

#include <memory>
#include <string>
#include <iostream>
#include <vulkan_algorithm.hpp>
#include <benchmark/benchmark.h>

decltype(auto) new_operation(int size) {
    auto n = new char [size];
    delete[] n;
}

decltype(auto) new_benchmark(benchmark::State& state) {
    for (auto _ : state) {
        new_operation(state.range(0));
    }
}

decltype(auto) malloc_operation(int size) {
    auto m = std::malloc(size);
    std::free(m);
}

decltype(auto) malloc_benchmark(benchmark::State& state) {
    for (auto _ : state) {
        malloc_operation(state.range(0));
    }
}

template<typename... _args>
[[nodiscard]]
decltype(auto) multi_arguments(_args&&... args) {
    auto _1 = 0;
    auto _2 = 0;
    ((_1 -= std::forward<_args>(args)), ...);
    ((_2 += std::forward<_args>(args)), ...);
    auto dummy = 0;
}

decltype(auto) multi_benchmark(benchmark::State& state) {
    for (auto _ : state) {
        multi_arguments(state.range(0), state.range(1), state.range(2), state.range(3));
    }
}

decltype(auto) dummy_benchmark(benchmark::State& state) {
    for (auto _ : state) {
        [](auto begin, auto end) {
        }(state.range(0), state.range(0));
    }
}

decltype(auto) self_argument(benchmark::internal::Benchmark* bm) {
    for (auto i = 100; i <= 1000; i += 100) {
        bm->Arg(i);
    }
}

BENCHMARK(multi_benchmark)->Args({ 1, 2, 3, 4 });
BENCHMARK(dummy_benchmark)->RangeMultiplier(10)->Range(1, 100)->Apply(self_argument);
BENCHMARK(new_benchmark)->Arg(1 << 7)->Arg(1 << 10)->Arg(1 << 20)->Iterations(1000);
BENCHMARK(malloc_benchmark)->Arg(1 << 7)->Arg(1 << 10)->Arg(1 << 20)->Iterations(1000);

BENCHMARK_MAIN();
