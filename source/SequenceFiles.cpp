#include "SequenceFiles.h"

#include <filesystem>
#include <fstream>
#include <random>

#include "Printer.h"

constexpr unsigned kDefaultSeed = 2718;
constexpr int kMinValue = 1;
constexpr int kMaxValue = 100000;

SequenceFiles::SequenceFiles(const std::vector<std::size_t>& counts, const std::string& dir)
    : m_counts(counts), m_dir(dir) {}
std::string SequenceFiles::pathFor(std::size_t count) const {
    return m_dir + "/numbers_" + std::to_string(count) + ".txt";
}

bool SequenceFiles::hasAllFiles() const {
    for (std::size_t count : m_counts)
        if (!std::filesystem::exists(pathFor(count)))
            return false;
    return true;
}

void SequenceFiles::write(std::size_t count, unsigned seed) const {
    std::mt19937 engine(seed);
    std::uniform_int_distribution<int> pick(kMinValue, kMaxValue);

    std::ofstream file(pathFor(count));
    file << count << '\n';
    for (std::size_t i = 0; i < count; ++i)
        file << pick(engine) << ' ';
    file << '\n';
}

void SequenceFiles::prepare(bool regenerate) const
{
    if (!regenerate && hasAllFiles()) {
        printInputRead(m_dir);
        return;
    }

    std::filesystem::create_directories(m_dir);
    const unsigned seed = regenerate ? std::random_device{}() : kDefaultSeed;
    for (std::size_t count : m_counts)
        write(count, seed);
    printInputWritten(m_dir, seed);
}

std::vector<int> SequenceFiles::read(std::size_t count) const {
    std::ifstream file(pathFor(count));
    std::size_t n = 0;
    file >> n;

    std::vector<int> values(n);
    for (int& v : values)
        file >> v;
    return values;
}
const std::vector<std::size_t>& SequenceFiles::counts() const {
    return m_counts;
}
