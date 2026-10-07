#pragma once

#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "Reducers.h"

inline const char* verdict(bool ok) {
    return ok ? "ok" : "WRONG";
}

inline void printHardwareThreads(unsigned threads) {
    std::cout << std::format("hardware threads: {}\n", threads);
}

inline void printInputRead(const std::string& dir) {
    std::cout << std::format("input: reading sequences from {}/\n\n", dir);
}

inline void printInputWritten(const std::string& dir, unsigned seed) {
    std::cout << std::format("input: wrote new sequences to {}/, seed {}\n\n", dir, seed);
}

inline void printLibraryTitle(int runs) {
    std::cout << "##### Tasks 1-2. std::reduce, library versions #####\n"
              << std::format("time = median of {} runs in ms, sequence already in memory\n", runs);
}

inline void printLibraryHeader(std::size_t count, double readMs) {
    std::string line = std::format("| {:<8} |", "op");
    for (ExecMode mode : kAllModes)
        line += std::format(" {:>9} |", modeLabel(mode));
    std::cout << std::format("\n-- {} numbers (reading the file took {:.3f} ms)\n", count, readMs)
              << line << std::format(" {:>15} | equal\n", "value");
}

inline void printLibraryRow(const char* op, const std::vector<double>& times, long long value, bool equal) {
    std::string line = std::format("| {:<8} |", op);
    for (double ms : times)
        line += std::format(" {:>9.3f} |", ms);
    std::cout << line << std::format(" {:>15} | {}\n", value, verdict(equal));
}

inline void printSplitTitle(std::size_t count) {
    std::cout << "\n##### Task 3. SplitReducer: K threads x sequential std::reduce, "
              << std::format("partial values joined by std::reduce; {} numbers #####\n", count);
}

inline void printSplitHeader(const char* op, int runs) {
    std::cout << std::format("\n[{}] median of {} runs\n", op, runs)
              << std::format("| {:>5} | {:>10} | {:>8} | {:>15} | equal\n", "K", "ms", "T1/TK", "value");
}

inline void printSplitRow(int k, double ms, double speedup, long long value, bool equal) {
    std::cout << std::format("| {:>5} | {:>10.3f} | {:>8.2f} | {:>15} | {}\n",
                             k, ms, speedup, value, verdict(equal));
}

inline void printSplitSummary(int bestK, double bestMs, int almostK, unsigned threads) {
    std::cout << std::format("fastest: K = {}, {:.3f} ms\n", bestK, bestMs)
              << std::format("hardware threads: {}, K / threads = {:.2f}\n", threads, 1.0 * bestK / threads)
              << std::format("first K within 5% of the fastest: {}, K / threads = {:.2f}\n",
                             almostK, 1.0 * almostK / threads);
}
