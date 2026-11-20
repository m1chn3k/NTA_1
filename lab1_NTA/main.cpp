#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;
using Clock = chrono::steady_clock;

static const uint64_t testNumbers[] = {
    3009182572376191ULL, 1021514194991569ULL, 4000852962116741ULL,
    15196946347083ULL, 499664789704823ULL, 269322119833303ULL,
    679321846483919ULL, 96267366284849ULL, 61333127792637ULL,
    2485021628404193ULL
};

static double seconds(Clock::time_point start) {
    return chrono::duration<double, milli>(Clock::now() - start).count();
}

static void logFactor(uint64_t factor, const string& method, Clock::time_point start) {
    cout << "Found: " << factor << " | " << method << " | " << seconds(start) << " ms\n";
}

static bool factorize(uint64_t n, vector<uint64_t>& factors, Clock::time_point start) {
    if (n == 1) return true;
    if (MillerRabin(n)) {
        factors.push_back(n);
        logFactor(n, "Miller-Rabin (prime)", start);
        return true;
    }

    uint64_t d = TrialDivision(n);
    string method = "Trial division";
    if (!d) {
        d = PollardRho(n);
        method = "Pollard Rho";
    }
    if (!d) {
        d = BrilhartMorrison(n);
        method = "Brilhart-Morrison";
    }
    if (d <= 1 || d >= n || n % d != 0) return false;

    logFactor(d, method, start);
    return factorize(d, factors, start) && factorize(n / d, factors, start);
}

int main(int argc, char* argv[]) {
    if (argc >= 2 && string(argv[1]) == "--bench") {
        cout << "Number,Pollard divisor,Pollard ms,CFRAC divisor,CFRAC ms\n";
        for (uint64_t value : testNumbers) {
            auto t1 = Clock::now();
            uint64_t p = PollardRho(value, 25, 100000);
            double pt = seconds(t1);
            auto t2 = Clock::now();
            uint64_t c = BrilhartMorrison(value);
            double ct = seconds(t2);
            cout << value << "," << p << "," << pt << "," << c << "," << ct << "\n";
        }
        return 0;
    }

    uint64_t n = 0;
    string mode;
    try {
        if (argc >= 2) n = stoull(argv[1]);
        else {
            cout << "Enter number (default variant 9: 341012868237902669): ";
            string input;
            getline(cin, input);
            n = input.empty() ? 341012868237902669ULL : stoull(input);
        }
        if (argc >= 3) mode = argv[2];
    } catch (...) {
        cerr << "Invalid number\n";
        return 1;
    }
    if (n < 2) {
        cerr << "Number must be >= 2\n";
        return 1;
    }

    time_t started = time(nullptr);
    cout << "Start: " << n << "\n";
    cout << "Started at: " << ctime(&started);
    auto start = Clock::now();

    if (mode == "--pollard" || mode == "--cfrac") {
        uint64_t d = mode == "--pollard" ? PollardRho(n, 25, 100000) : BrilhartMorrison(n);
        cout << "Method: " << (mode == "--pollard" ? "Pollard Rho" : "Brilhart-Morrison") << "\n";
        cout << "Divisor: " << d << "\n";
        cout << "Time: " << seconds(start) << " ms\n";
        return d > 1 && d < n ? 0 : 2;
    }
    if (!mode.empty()) {
        cerr << "Unknown mode\n";
        return 1;
    }

    vector<uint64_t> factors;
    bool ok = factorize(n, factors, start);
    if (!ok) {
        cerr << "Cannot find canonical decomposition :(\n";
        return 2;
    }
    sort(factors.begin(), factors.end());
    cout << "Result: ";
    for (size_t i = 0; i < factors.size(); ++i) {
        if (i) cout << " * ";
        cout << factors[i];
    }
    cout << "\nTotal time: " << seconds(start) << " ms\n";
    time_t finished = time(nullptr);
    cout << "Finished at: " << ctime(&finished);
    return 0;
}
