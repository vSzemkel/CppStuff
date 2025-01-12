
#include <iostream>
#include <limits>
#include <random>


static std::mt19937 g_gen{std::random_device{}()};

template <typename T = int>
static auto distribution(const T ubound) {
    std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist;
}

template <typename T = int>
static T rand_in_range(const T ubound) {
    static std::random_device seed;
    static std::mt19937 gen{seed()};
    static std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist(gen);
};

static bool flip_a_coin() {
    const int rv = rand_in_range<>(std::numeric_limits<int>::max());
    return rv & 1;
}

int main(int, char**)
{
    std::cout << "Some random int64_t values from common distribution:\n";
    auto dist = distribution<int64_t>(std::numeric_limits<int>::max());
    for (int z = 7; z; --z)
        std::cout << dist(g_gen) << ", ";
    std::cout << "\nSome random int values:\n";
    for (int z = 7; z; --z)
        std::cout << rand_in_range<>(std::numeric_limits<int>::max()) << ", ";
    std::cout << "\n\nSome random booleans:\n";
    for (int r = 20; r; --r) {
        for (int c = 80; c; --c) 
            std::cout << flip_a_coin();
        std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 randomness.cpp -o randomness.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address randomness.cpp -o randomness

*/
