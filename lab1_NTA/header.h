#pragma once
#include <cstdint>
#include <vector>

uint64_t gcd64(uint64_t a, uint64_t b);
uint64_t addMod(uint64_t a, uint64_t b, uint64_t mod);
uint64_t mulMod(uint64_t a, uint64_t b, uint64_t mod);
uint64_t powMod(uint64_t a, uint64_t e, uint64_t mod);
bool MillerRabin(uint64_t n);
uint64_t TrialDivision(uint64_t n);
uint64_t PollardRho(uint64_t n, int attempts = 8, int limit = 30000);
uint64_t BrilhartMorrison(uint64_t n);
