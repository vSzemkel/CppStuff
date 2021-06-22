#include <iostream>
#include <random>
#include <vector>

const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

const size_t g_size = 300;
std::vector<int64_t> g_input;

void fuzz()
{
    int g_D = 20;
    g_input.resize(g_size);

    int errors{0};
    int trials{1000};
    for (int s = 0; s < trials; ++s) {
        for (int i = 0; i < g_size; ++i)
            g_input[i] = 1 + rand_in_range(1000);
        g_input[g_size / 2] = 300'000'000'000;
        const auto  s1 = solve();
        const auto  s2 = solve_slow();
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
