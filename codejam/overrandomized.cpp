
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Overrandomized
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fef4/00000000003179a1

const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

constexpr int g_samples = 10'000;

std::string solve() {
    int digits; std::cin >> digits;
    std::unordered_map<char, uint64_t> stats;
    for (int i = 0; i < g_samples; ++i) {
        int64_t n;
        std::string r;
        std::cin >> n >> r;
        ++stats[r[0]];
        if (r.size() > 1)
            ++stats[r[1]];
    }

    std::map<uint64_t, char, std::greater<>> switched_stats;
    for (const auto& s : stats)
        switched_stats[s.second] = s.first;

    std::string ret;
    for (const auto& s : switched_stats)
        ret += s.second;

    std::rotate(ret.begin(), ret.end() - 1, ret.end());
    return ret;
}

void probe(const int digits)
{
    std::map<char, uint64_t> stats;
    const uint64_t max_value = pow(10, digits);
    for (uint64_t n = 2; n <= max_value; ++n)
        for (const char d : std::to_string(rand_in_range(n)))
            ++stats[d];

    for (const auto& s : stats)
        std::cout << s.first << ": " << s.second << "\n";
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // probe(9); return 0;

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << solve() << "\n";
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 overrandomized.cpp -o overrandomized.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 overrandomized.cpp -o overrandomized.o

Run:
overrandomized.exe < overrandomized.in

*/
