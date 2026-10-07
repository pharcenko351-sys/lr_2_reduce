// GCC (MSYS2, MinGW-w64 x86_64).
// Polina Harchenko K-28.

#include <string_view>
#include <thread>

#include "Benchmark.h"
#include "SequenceFiles.h"
#include "Printer.h"

constexpr int kRuns = 15;

int main(int argc, char* argv[]) {
    bool regenerate = false;
    for (int i = 1; i < argc; ++i)
        if (std::string_view(argv[i]) == "--regen")
            regenerate = true;

    printHardwareThreads(std::thread::hardware_concurrency());

    const SequenceFiles files({10'000, 100'000, 1'000'000, 10'000'000});
    files.prepare(regenerate);

    const Benchmark bench(files, kRuns);
    bench.libraryPart();
    bench.customPart();
    return 0;
}
