#include <iostream>
#include <random>
#include <vector>

const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

constexpr size_t g_size = 300;
constexpr int g_trials = 1000;
constexpr bool g_unique = true;
constexpr int g_values_range = 1000;
std::vector<int64_t> g_input(g_size);

void fuzz()
{
    // for unique input values
    std::random_device rd;
    std::mt19937 rg(rd());
    if (g_unique)
        std::iota(g_input.begin(), g_input.end(), 0);

    int errors{0};
    for (int s = 0; s < g_trials; ++s) {
        if (g_unique)
            std::shuffle(g_input.begin(), g_input.end(), rg);
        else
            for (int i = 0; i < g_size; ++i)
                g_input[i] = 1 + rand_in_range(g_values_range);

        const auto s1 = solve();
        const auto s2 = solve_set1();
        if (s1 != s2) {
            ++errors;
            std::cout << s1 << ' ' << s2 << '\n';
            for (int i = 0; i < g_size; ++i)
                std::cout << g_input[i] << ' ';
            std::cout << std::endl;
        }
    }

    std::cout << "In " << trials << " trials encountered " << errors << " errors.\n";
}
