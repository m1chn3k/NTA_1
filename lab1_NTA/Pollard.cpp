#include "header.h"
#include <random>

uint64_t TrialDivision(uint64_t n) {
    const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    for (int p : primes) if (n % p == 0) return p;
    return 0;
}

uint64_t PollardRho(uint64_t n, int attempts, int limit) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    if (n < 5 || MillerRabin(n)) return 0;

    static std::mt19937_64 rng(std::random_device{}());
    for (int t = 0; t < attempts; ++t) {
        uint64_t x = 2 + rng() % (n - 3);
        uint64_t y = x;
        uint64_t c = 1 + rng() % (n - 1);
        auto next = [&](uint64_t v) {
            return addMod(mulMod(v, v, n), c, n);
        };
        for (int i = 0; i < limit; ++i) {
            x = next(x);
            y = next(next(y));
            uint64_t diff = x > y ? x - y : y - x;
            uint64_t d = gcd64(diff, n);
            if (d == 1) continue;
            if (d != n) return d;
            break;
        }
    }
    return 0;
}
