#include <benchmark/benchmark.h>

#include "std_for_each_speed_test.h"

template<typename T, class execution_policy>
static void std_for_each_benchmark(benchmark::State& state, T global_size, execution_policy&& policy) {
    auto container = container_t<T>{};
    fill_container(global_size, container);
    for (auto _ : state) {
        std::atomic<T> sum{ 0 };
        std::for_each(
            policy, std::begin(container), std::end(container),
            [&](const std::vector<T>& local_container) {
            auto local_sum = T{ 0 };
            std::for_each(
                std::execution::seq, std::begin(local_container),
                std::end(local_container), [&](const T& value) {
                local_sum += value;
            });
            sum += local_sum;
        });
    }
}

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

    if constexpr(sizeof(void*) >= 8) {
        BENCHMARK_CAPTURE(std_for_each_benchmark, std::execution::seq, int64_t{ 536870912 }, std::execution::seq);
        BENCHMARK_CAPTURE(std_for_each_benchmark, std::execution::par, int64_t{ 536870912 }, std::execution::par);
        BENCHMARK_CAPTURE(std_for_each_benchmark, std::execution::par_unseq, int64_t{ 536870912 }, std::execution::par_unseq);
    }
    else {
        BENCHMARK_CAPTURE(std_for_each_benchmark, std::execution::seq, int32_t{ 268435456 }, std::execution::seq);
        BENCHMARK_CAPTURE(std_for_each_benchmark, std::execution::par, int32_t{ 268435456 }, std::execution::par);
        BENCHMARK_CAPTURE(std_for_each_benchmark, std::execution::par_unseq, int32_t{ 268435456 }, std::execution::par_unseq);
    }

    benchmark::RunSpecifiedBenchmarks();
    std::cout << "\n**Press enter to exit.**";
    std::string wait_for_input;
    std::getline(std::cin, wait_for_input);
}