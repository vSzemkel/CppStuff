
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <numeric>
#include <vector>

// Merge Cards
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000001a0069/0000000000415054

std::vector<int64_t> g_input;
std::vector<int64_t> g_partial;
std::unordered_map<int, double> g_cache;

int64_t partial_sum(const int i, const int j)
{
    return g_partial[j + 1] - g_partial[i];
}

double brute_force(const std::vector<int64_t>& input)
{
    const auto size = input.size();
    if (size == 2)
        return static_cast<double>(input[0] + input[1]);

    double ret{0.0};
    for (int i = 0; i < size - 1; ++i) {
        auto copy = input;
        auto n = copy[i + 1] += copy[i];
        copy.erase(copy.begin() + i, copy.begin() + i + 1);
        ret += static_cast<double>(n) + brute_force(copy);
    }

    return ret / (size - 1);
}

void merge_cards(const std::vector<int64_t>& input, double* accu)
{
    const auto size = input.size();
    if (size == 1)
        return;
    if (size == 2) {
        *accu += input[0] + input[1];
        return;
    }

    double ac{0};
    for (int i = 1; i < size; ++i) {
        merge_cards(std::vector<int64_t>(input.begin(), input.begin() + i), &ac);
        merge_cards(std::vector<int64_t>(input.begin() + i, input.end()), &ac);
    }

    const int64_t partial = std::accumulate(input.begin(), input.end(), int64_t{});
    *accu += (double)partial + (ac / (size - 1));
}

double merge_cards2(const int i, const int j)
{
    const auto size = j - i;
    if (size == 1) return 0;

    const auto cache_index = (i << 16) + j;
    const auto cache_it = g_cache.find(cache_index);
    if (cache_it != g_cache.end()) return cache_it->second;

    double acc{0.0};
    for (int k = i + 1; k < j; ++k) {
        acc += merge_cards2(i, k);
        acc += merge_cards2(k, j);
    }

    const auto ret = (double)partial_sum(i, j - 1) + (acc / (size - 1));
    g_cache[cache_index] = ret;
    return ret;
}

double merge_cards3(const std::vector<int64_t>& input)
{
    const auto size = input.size();
    std::vector<std::vector<double>> dp(size, std::vector<double>(size, 0.0));
    std::vector<double> pref(size, 0.0), suff(size, 0.0);
    for (int len = 2; len <= size; ++len)
        for (int i = 0, j = i + len - 1; j < size; ++i, ++j) {
            double& chunk = dp[i][j];
            chunk += (pref[i] + suff[j]) / (len - 1); 
            chunk += partial_sum(i, j);
            pref[i] += chunk;
            suff[j] += chunk;
        }

    return dp[0][size - 1];
}

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases, size;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> size;
        g_input.resize(size);
        for (auto& c : g_input) std::cin >> c;
        // Brute Force
        //std::cout << "Case #" << g << ": " << std::setprecision(15) << brute_force(g_input) << "\n";
        // Set1
        //double accu{0.0};
        //merge_cards(g_input, &accu);
        //std::cout << "Case #" << g << ": " << std::setprecision(15) << accu << "\n";
        // Set2
        g_partial = {0};
        g_cache.clear();
        std::inclusive_scan(g_input.begin(), g_input.end(), std::back_inserter(g_partial));
        //std::cout << "Case #" << g << ": " << std::setprecision(15) << merge_cards2(0, size) << "\n";
        // Set3
        std::cout << "Case #" << g << ": " << std::setprecision(15) << merge_cards3(g_input) << "\n";
    }
}


/* clang++.exe -Wall -ggdb3 -O0 -std=c++17 merge_cards.cpp -o merge_cards.exe
merge_cards.exe < merge_cards.in

Input:

4
3
2 1 10
5
19 3 78 2 31
7
19 80 3 78 2 31 20
11
19 6152439 80 55555555 765 0 78 31 23 911 90

Output:

Case #1: 20
Case #2: 352.333333333333
Case #3: 763.916666666667
Case #4: 269465162.754762

*/