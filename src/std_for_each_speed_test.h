#ifndef __STD_FOR_EACH_SPEED_TEST__
#define __STD_FOR_EACH_SPEED_TEST__
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

template<typename T>
void fill_container(T global_size, container_t<T> &container)
{
    auto concurrency =
        static_cast<T>(std::thread::hardware_concurrency() - 1);
    concurrency = std::max(concurrency, T{ 1 });
    T local_size = global_size / concurrency;

    for (auto i = 0; i < concurrency; ++i) {
        container.push_back(std::vector<T>{ local_size });
        container[i].resize(local_size);
        std::fill(
            std::execution::par_unseq,
            std::begin(container[i]),
            std::end(container[i]),
            2);
    }
}
#endif