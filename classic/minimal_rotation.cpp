
#include <algorithm>
#include <cassert>
#include <string>
#include <random>

// Minimal Rotation
// Find lexicographically minimal rotation of a string


static std::mt19937 g_gen{std::random_device{}()};

static int rand_in_range(const int ubound) {
    std::uniform_int_distribution<int> dist(0, ubound - 1);
    return dist(g_gen);
};

static int solve(std::string s)
{
    const int sz = int(s.size());
    int best{};
    s += s;
    for (int can = 1; can < sz; ++can)
        for (int i = 0; i < sz; ++i)
            if (best + i == can || s[best + i] < s[can + i]) {
                can += i;
                break;
            } else if (s[can + i] < s[best + i]) {
                best = can;
                break;
            }

    return best;
}

int main(int, char**)
{
    const int SZ = 100'000;
    std::string s(SZ, {});
    for (char& c : s)
        c = 'a' + rand_in_range(26);

    auto t = s;
    std::rotate(t.begin(), t.begin() + solve(s), t.end());
    for (int i = 0; i < SZ; ++i) {
        assert(t <= s);
        std::rotate(s.begin(), s.begin() + 1, s.end());
    }
}

/*

Format:
clang-format -i minimal_rotation.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 minimal_rotation.cpp
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address minimal_rotation.cpp
clang -Wall -Wextra -Og -ggdb3 -lstdc++ -std=c++20 minimal_rotation.cpp

*/
