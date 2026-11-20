# NTA Lab 1 — Variant 9

C++17 implementation of integer factorization.

Open `lab1_NTA.sln` in Visual Studio 2026 and run the x64 project. Press Enter to factor the default number, or enter a different number.

Variant 9: `341012868237902669 = 23 * 1549 * 937033 * 10214959`

Command-line modes:
- `lab1_NTA.exe 341012868237902669` — factorization
- `lab1_NTA.exe 3009182572376191 --pollard` — Pollard Rho
- `lab1_NTA.exe 3009182572376191 --cfrac` — Brilhart-Morrison
- `lab1_NTA.exe --bench` — timings for the ten numbers in the assignment

For performance measurements, build in Release x64.

Optional Docker build from `lab1_NTA` directory:

```sh
docker build -t nta-lab1 .
docker run --rm nta-lab1 341012868237902669
```

The implementation uses Miller-Rabin, trial division, Pollard Rho and Brilhart-Morrison. It handles unsigned 64-bit numbers and does not guarantee factoring arbitrary hard semiprimes within its search limits.
