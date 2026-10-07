#pragma once

#include <algorithm>
#include <chrono>
#include <vector>

template <typename Func>

double elapsedMs(Func func) {
    const auto start = std::chrono::steady_clock::now();
    func();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}
    // один запуск

template <typename Func>
double medianMs(Func func, int runs) {
    func();
    std::vector<double> times;
    for (int i = 0; i < runs; ++i)
        times.push_back(elapsedMs(func));
    std::sort(times.begin(), times.end());
    return times[runs / 2];
}

