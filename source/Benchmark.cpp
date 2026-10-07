#include "Benchmark.h"

#include <algorithm>
#include <thread>

#include "BinaryOps.h"
#include "Printer.h"
#include "Reducers.h"
#include "Stopwatch.h"

const std::vector<int> kPieceCounts = {1,  2,  3,  4,  5,   6,   7,   8,   10,
                                       12, 16, 24, 32, 64, 128, 256, 512, 1024};

constexpr double kAcceptableSlowdown = 1.05;

// я а тебе люблю дуже зай)

static void reportBestK(const std::vector<double>& times) {
    const auto best = std::min_element(times.begin(), times.end());
    const auto almost = std::find_if(times.begin(), times.end(),
                                     [&](double ms) { return ms <= *best * kAcceptableSlowdown; });
    printSplitSummary(kPieceCounts[best - times.begin()], *best,
                      kPieceCounts[almost - times.begin()], std::thread::hardware_concurrency());
}

Benchmark::Benchmark(const SequenceFiles& files, int runs) : m_files(files), m_runs(runs) {}

template <typename ReduceCall>

Benchmark::Measurement Benchmark::measure(ReduceCall&& reduceCall) const {
    Measurement result;
    result.ms = medianMs([&] { result.value = reduceCall(); }, m_runs);
    return result;
}

void Benchmark::libraryPart() const {
    printLibraryTitle(m_runs);

    for (std::size_t count : m_files.counts()) {
        std::vector<int> values;
        const double readMs = elapsedMs([&] { values = m_files.read(count); });

        printLibraryHeader(count, readMs);
        applyToEachOp([&](auto op) { libraryRow(values, op); });
    }
}

template <typename Op>

void Benchmark::libraryRow(const std::vector<int>& values, Op op) const {
    const long long reference = stdReduce(ExecMode::NoPolicy, values, op);
    std::vector<double> times;
    bool allEqual = true;

    for (ExecMode mode : kAllModes) {
        const Measurement m = measure([&] { return stdReduce(mode, values, op); });
        times.push_back(m.ms);
        allEqual = allEqual && m.value == reference;
    }
    printLibraryRow(Op::label, times, reference, allEqual);
}

void Benchmark::customPart() const {
    const std::size_t count = m_files.counts().back();
    const std::vector<int> values = m_files.read(count);

    printSplitTitle(count);
    applyToEachOp([&](auto op)
        { splitTable(values, op); });
}

template <typename Op>
void Benchmark::splitTable(const std::vector<int>& values, Op op) const {
    const long long reference = stdReduce(ExecMode::NoPolicy, values, op);
    printSplitHeader(Op::label, m_runs);

    std::vector<double> times;
    for (int k : kPieceCounts) {
        const SplitReducer reducer(k);
        const Measurement m = measure([&] { return reducer.run(values, op); });
        times.push_back(m.ms);
        printSplitRow(k, m.ms, times.front() / m.ms, m.value, m.value == reference);
    }
    reportBestK(times);
}
