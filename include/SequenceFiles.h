#pragma once

#include <cstddef>
#include <string>
#include <vector>

class SequenceFiles {
public:
    explicit SequenceFiles(const std::vector<std::size_t>& counts, const std::string& dir = "data");

    void prepare(bool regenerate) const;

    std::vector<int> read(std::size_t count) const;
    const std::vector<std::size_t>& counts() const;

private:
    std::string pathFor(std::size_t count) const;
    bool hasAllFiles() const;
    void write(std::size_t count, unsigned seed) const;

    std::vector<std::size_t> m_counts;
    std::string m_dir;
};
