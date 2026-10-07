#pragma once

#include <array>
#include <execution>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

enum class ExecMode { NoPolicy, Seq, Unseq, Par, ParUnseq };

inline constexpr std::array<ExecMode, 5> kAllModes = {
    ExecMode::NoPolicy, ExecMode::Seq, ExecMode::Unseq, ExecMode::Par, ExecMode::ParUnseq};

std::string modeLabel(ExecMode mode);

template <typename Op>
long long stdReduce(ExecMode mode, const std::vector<int>& values, Op op) {
    const auto begin = values.begin();
    const auto end = values.end();
    switch (mode) {
    case ExecMode::Seq:      return std::reduce(std::execution::seq, begin, end, Op::neutral, op);
    case ExecMode::Unseq:    return std::reduce(std::execution::unseq, begin, end, Op::neutral, op);
    case ExecMode::Par:      return std::reduce(std::execution::par, begin, end, Op::neutral, op);
    case ExecMode::ParUnseq: return std::reduce(std::execution::par_unseq, begin, end, Op::neutral, op);
    case ExecMode::NoPolicy: break;
    }
    return std::reduce(begin, end, Op::neutral, op);
}

class SplitReducer {
public:
    explicit SplitReducer(int parts) : m_parts(parts) {}

    template <typename Op>
    long long run(const std::vector<int>& values, Op op) const {
        std::vector<long long> partial(m_parts, Op::neutral);
        std::vector<std::thread> threads;

        for (int i = 0; i < m_parts; ++i)
            threads.emplace_back([&, i]
            {
                partial[i] = reducePart(values, i, op);
            });

        for (std::thread& t : threads)
            t.join();

        return std::reduce(partial.begin(), partial.end(), Op::neutral, op);
    }

private:
    template <typename Op>
    long long reducePart(const std::vector<int>& values, int index, Op op) const {
        const std::size_t size = values.size() / m_parts;
        const std::size_t first = index * size;
        const std::size_t last = (index == m_parts - 1) ? values.size() : first + size;
        return std::reduce(values.begin() + first, values.begin() + last, Op::neutral, op);
    }

    int m_parts;
};


