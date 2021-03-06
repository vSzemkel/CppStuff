std::vector<int64_t> factor10(length);

// precompute factor10 up to the longest number
for (int i = 0; i < length; ++i) {
    factor10[i] = (i == 0) ? 1 : 10 * factor10[i - 1];

assert(0 <= n && n < std::floor(log10(n)) + 1);
nth_digit = d / factor10[n] % 10 // n == 0 for less significant
