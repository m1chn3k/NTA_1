#include "header.h"
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;

struct Relation {
    uint64_t x;
    vector<int> powers;
};

static bool isSmallPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

static vector<int> makeBase(uint64_t n, int bound) {
    vector<int> base = {-1, 2};
    for (int p = 3; p <= bound; p += 2) {
        if (isSmallPrime(p) && powMod(n % p, (p - 1) / 2, p) == 1)
            base.push_back(p);
    }
    return base;
}

static bool smooth(uint64_t value, bool negative, const vector<int>& base, vector<int>& powers) {
    powers.assign(base.size(), 0);
    powers[0] = negative ? 1 : 0;
    for (size_t j = 1; j < base.size(); ++j) {
        int p = base[j];
        while (value % p == 0) {
            ++powers[j];
            value /= p;
        }
        if (value == 1) break;
    }
    return value == 1;
}

static uint64_t tryRelations(uint64_t n, const vector<int>& base, const vector<Relation>& rel) {
    int cols = (int)base.size();
    int rows = (int)rel.size();
    int blocks = (rows + 63) / 64;
    vector<vector<uint64_t>> pivots(cols);
    vector<vector<uint64_t>> choices(cols);

    for (int i = 0; i < rows; ++i) {
        vector<uint64_t> combination(blocks, 0);
        combination[i / 64] = uint64_t(1) << (i % 64);
        vector<unsigned char> parity(cols);
        for (int j = 0; j < cols; ++j) parity[j] = rel[i].powers[j] & 1;

        bool dependent = true;
        for (int col = 0; col < cols; ++col) {
            if (!parity[col]) continue;
            if (pivots[col].empty()) {
                pivots[col] = vector<uint64_t>((cols + 63) / 64, 0);
                for (int j = col; j < cols; ++j)
                    if (parity[j]) pivots[col][j / 64] |= uint64_t(1) << (j % 64);
                choices[col] = move(combination);
                dependent = false;
                break;
            }
            for (int j = col; j < cols; ++j)
                parity[j] ^= (pivots[col][j / 64] >> (j % 64)) & 1;
            for (int j = 0; j < blocks; ++j) combination[j] ^= choices[col][j];
        }
        if (!dependent) continue;

        vector<int> powers(cols, 0);
        uint64_t x = 1;
        for (int j = 0; j < rows; ++j) {
            if ((combination[j / 64] >> (j % 64)) & 1) {
                x = mulMod(x, rel[j].x, n);
                for (int k = 0; k < cols; ++k) powers[k] += rel[j].powers[k];
            }
        }
        uint64_t y = 1;
        for (int j = 1; j < cols; ++j)
            if (powers[j] >= 2)
                y = mulMod(y, powMod(base[j], powers[j] / 2, n), n);
        if ((powers[0] / 2) & 1) y = n - y;
        if (mulMod(x, x, n) != mulMod(y, y, n)) continue;
        uint64_t d = gcd64(x > y ? x - y : y - x, n);
        if (d > 1 && d < n) return d;
        d = gcd64(addMod(x, y, n), n);
        if (d > 1 && d < n) return d;
    }
    return 0;
}

uint64_t BrilhartMorrison(uint64_t n) {
    if (n < 4 || MillerRabin(n)) return 0;
    if (n % 2 == 0) return 2;
    uint64_t root = (uint64_t)sqrtl((long double)n);
    while ((root + 1) <= n / (root + 1)) ++root;
    while (root > n / root) --root;
    if (root * root == n) return root;

    double ln = log((double)n);
    int bound = (int)exp(sqrt(ln * log(ln)) / sqrt(2.0));
    bound = max(250, min(bound, 3000));
    vector<int> base = makeBase(n, bound);
    vector<Relation> rel;
    int target = (int)base.size() + 12;

    uint64_t m = 0, d = 1, a = root;
    uint64_t prev2 = 0, prev1 = 1;
    for (int i = 0; i < 300000; ++i) {
        uint64_t x = addMod(mulMod(a, prev1, n), prev2, n);
        uint64_t nextM = d * a - m;
        uint64_t nextD = (n - nextM * nextM) / d;
        if (nextD == 0) break;
        uint64_t nextA = (root + nextM) / nextD;

        vector<int> powers;
        if (smooth(nextD, i % 2 == 0, base, powers)) {
            rel.push_back({x, move(powers)});
            if ((int)rel.size() >= target) {
                uint64_t found = tryRelations(n, base, rel);
                if (found) return found;
                target += 12;
            }
        }
        m = nextM;
        d = nextD;
        a = nextA;
        prev2 = prev1;
        prev1 = x;
    }
    return 0;
}
