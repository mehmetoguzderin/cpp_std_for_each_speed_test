#include <algorithm>
#include <atomic>
#include <chrono>
#include <execution>
#include <iostream>
#include <string>
#include <vector>

template<typename T>
using container_t =
std::vector<std::vector<T>>;

template<class execution_policy, typename T>
std::tuple<double, T> calculate_sum(
    execution_policy&& policy, container_t<T>& container) {
    std::atomic<T> sum{ 0 };
    auto start_time = std::chrono::steady_clock::now();
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
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast
        <std::chrono::duration<double, std::milli>>(end_time - start_time);
    return { duration.count(), sum };
};

template<typename T>
void speed_test(T global_size) {
    auto concurrency =
        static_cast<T>(std::thread::hardware_concurrency() - 1);
    concurrency = std::max(concurrency, T{ 1 });
    T local_size = global_size / concurrency;

    auto container = container_t<T>{};
    for (auto i = 0; i < concurrency; ++i) {
        container.push_back(std::vector<T>{ local_size });
        container[i].resize(local_size);
        std::fill(
            std::execution::par_unseq,
            std::begin(container[i]),
            std::end(container[i]),
            2);
    }

    auto[seq_duration, seq_sum] =
        calculate_sum(std::execution::seq, container);
    auto[par_duration, par_sum] =
        calculate_sum(std::execution::par, container);
    auto[par_unseq_duration, par_unseq_sum] =
        calculate_sum(std::execution::par_unseq, container);
    std::cout << "# std::for_each Speed Test Results\n";
    std::cout << "\n## Sequenced Execution" <<
        "\n* Duration (ms): " << seq_duration <<
        "\n* Sum: " << seq_sum << "\n";
    std::cout << "\n## Parallel Execution" <<
        "\n* Duration (ms): " << par_duration <<
        "\n* Sum: " << par_sum << "\n";
    std::cout << "\n## Parallel Unsequenced Execution" <<
        "\n* Duration (ms): " << par_unseq_duration <<
        "\n* Sum: " << par_unseq_sum << "\n";
}

int main() {
    if constexpr(sizeof(void*) >= 8) {
        speed_test<int64_t>(536870912);
    }
    else {
        speed_test<int32_t>(268435456);
    }
    std::cout << "\n**Press enter to exit.**";
    std::string wait_for_input;
    std::getline(std::cin, wait_for_input);
    return 0;
}