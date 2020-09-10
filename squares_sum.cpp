
#include <iostream>
#include <map>

// find all positive numbers a, b, c, d; a <= b, c <=d, not greater then bound
// that a*a + b*b == c*c + d*d


// case definition
constexpr int g_bound = 500;

// globals
std::multimap<int, std::pair<int, int>> g_pairs;

int main(int argc, char* argv[])
{
    for (int f = 1; f <= g_bound; ++f)
        for (int s = f; s <= g_bound; ++s)
            g_pairs.emplace(f*f + s*s, std::make_pair(f, s));

    int last_sum = 0;
    for (const auto& sum : g_pairs)
        if (sum.first != last_sum && g_pairs.count(sum.first) > 1) {
            const auto& pairs = g_pairs.equal_range(sum.first);
            printf("%i:", sum.first);
            for (auto it = pairs.first; it != pairs.second; ++it)
                printf(" [%i, %i]", it->second.first, it->second.second);
            printf("\n");
            last_sum = sum.first;
        }

    return 0;
}

/* clang++.exe -Wall -g -std=c++17 squares_sum.cpp -o squares_sum.exe

Output (digest):

..
333125: [305, 490] [322, 479] [335, 470] [385, 430]
...
428810: [431, 493] [457, 469]
431525: [433, 494] [463, 466]
433381: [434, 495] [465, 466]
434762: [431, 499] [451, 481]
439097: [436, 499] [461, 476]

*/