#pragma once

#include <vector>

#include "SequenceFiles.h"

class Benchmark {
public:
    Benchmark(const SequenceFiles& files, int runs);

    void libraryPart() const;

    void customPart() const;

private:
    struct Measurement {
        double ms = 0;
        long long value = 0;
    };

    template <typename Op>
    void libraryRow(const std::vector<int>& values, Op op) const;

    template <typename Op>
    void splitTable(const std::vector<int>& values, Op op) const;

    template <typename ReduceCall>
    Measurement measure(ReduceCall&& reduceCall) const;

    const SequenceFiles& m_files;
    int m_runs;
};
