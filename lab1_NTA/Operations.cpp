#include "header.h"
#include <numeric>

uint64_t gcd64(uint64_t a, uint64_t b) {
    return std::gcd(a, b);
}

uint64_t addMod(uint64_t a, uint64_t b, uint64_t mod) {
    a %= mod;
    b %= mod;
    return a >= mod - b ? a - (mod - b) : a + b;
}

uint64_t mulMod(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t result = 0;
    a %= mod;
    while (b) {
        if (b & 1) result = addMod(result, a, mod);
        b >>= 1;
        if (b) a = addMod(a, a, mod);
    }
    return result;
}

uint64_t powMod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t result = 1;
    while (e) {
        if (e & 1) result = mulMod(result, a, mod);
        e >>= 1;
        if (e) a = mulMod(a, a, mod);
    }
    return result;
}
