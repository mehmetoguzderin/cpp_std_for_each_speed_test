#include <algorithm>
#include <atomic>
#include <chrono>
#include <execution>
#include <iostream>
#include <string>
#include <vector>

using container_t =
std::vector<std::vector<int64_t>>;
template<class execution_policy>
std::tuple<double, int64_t> calculate_sum(
    execution_policy&& policy, container_t& container) {
    std::atomic<int64_t> sum{ 0 };
    auto start_time = std::chrono::steady_clock::now();
    std::for_each(
        policy, std::begin(container), std::end(container),
        [&](const std::vector<int64_t>& local_container) {
        auto local_sum = int64_t{ 0 };
        std::for_each(
            std::execution::seq, std::begin(local_container),
            std::end(local_container), [&](const int64_t& value) {
            local_sum += value;
        });
        sum += local_sum;
    });
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast
        <std::chrono::microseconds>(end_time - start_time);
    return { double(duration.count()) * 0.001, sum };
};

void speed_test() {
    auto concurrency =
        int64_t{ std::thread::hardware_concurrency() - 1 };
    concurrency = std::max(concurrency, int64_t{ 1 });
    auto local_size = 536870912 / concurrency;
    auto container = container_t{};
    for (auto i = 0; i < concurrency; ++i) {
        container.push_back(std::vector<int64_t>{ local_size });
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
    speed_test();
    std::cout << "\n**Press enter to exit.**";
    std::string wait_for_input;
    std::getline(std::cin, wait_for_input);
    return 0;
}