#include "header.h"

bool MillerRabin(uint64_t n) {
    if (n < 2) return false;
    const uint64_t small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (uint64_t p : small) {
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (uint64_t a : bases) {
        if (a % n == 0) continue;
        uint64_t x = powMod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool passed = false;
        for (int i = 1; i < s; ++i) {
            x = mulMod(x, x, n);
            if (x == n - 1) {
                passed = true;
                break;
            }
        }
        if (!passed) return false;
    }
    return true;
}
