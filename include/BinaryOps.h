#pragma once

struct AddOp {
    static constexpr const char* label = "add";
    static constexpr long long neutral = 0;

    long long operator()(long long a, long long b) const { return a + b; }
};

struct MulModOp {
    static constexpr const char* label = "mul_mod";
    static constexpr long long neutral = 1;

    long long operator()(long long a, long long b) const { return a * b % 1'000'000'007; }
};

template <typename Func>
void applyToEachOp(Func func) {
    func(AddOp{});
    func(MulModOp{});
}
