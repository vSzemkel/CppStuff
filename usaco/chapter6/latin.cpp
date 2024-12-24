/*
ID: marcin.8
LANG: C++
TASK: latin
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=q5IyqPflpNO&S=latin
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <numeric>
#include <unordered_map>
#include <vector>

std::ifstream task_in("latin.in");
std::ofstream task_out("latin.out");

int N, required_rounds;
int64_t count_21star, ans{};
std::vector<int> rows;
std::unordered_map<uint64_t, int64_t> memo; // cache ignores left column
static const int factorial[] = {1, 1, 2, 6, 24, 120, 720};
static const int factor10[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

int nth_digit(int number, int n)
{
    return number / factor10[n] % 10;
}

bool try_set_cache(uint64_t& cache, const int digit, const int column)
{
    assert(0 < digit && digit < 8 && 1 <= column && column < 8);
    auto ptr = reinterpret_cast<uint8_t*>(&cache) + column - 1;
    const int mask = 1 << (digit - 1);
    if (*ptr & mask)
        return false;
    *ptr |= mask;
    return true;
}

std::vector<int> valid_candidates(const uint64_t cache, const int head)
{
    assert(1 < head && head < 8);
    auto ptr = reinterpret_cast<const uint8_t*>(&cache);
    std::vector<int> ret(1, head);

    auto no_such_digit = [](int number, int digit){
        for (; number; number /= 10)
            if (number % 10 == digit)
                return false;
        return true;
    };
    for (int pos = 1; pos < N; ++pos, ++ptr) {
        std::vector<int> tmp;
        for (const auto p : ret)
            for (int d = 1, m = 1; d <= N; ++d, m <<= 1)
                if ((*ptr & m) == 0 && no_such_digit(p, d))
                    tmp.push_back(10 * p + d);
        std::swap(ret, tmp);
    }

    return ret;
}

/**
 * Observation1: if we find acceptable latin square with sorted rows, we can have (N - 1)! variants of it by permuting all rows but the first one.
 * Observation2: Acceptable, row-sorted squares have left column fixed and equal to top row
 * Observation3: Correctly filled N-1 square can be extended to full N size in only one way
 * Observation4: Computation for second row can by optimized, see coment inline below
 */
void inner_solve(const int round, uint64_t cache)
{
    if (round == required_rounds) {
        ++ans;
        return;
    }

    for (const auto can : valid_candidates(cache, round + 1)) {
        if (round == 1) {
            // Observation4 : optimization for second row
            // 21* is fewer then 23* because for 21* digit 3 can not be anywhere in *, for 23* digit 1 can be anywhere
            // 23* and 24* are symmetric in any mean, the same hold true for 25* .. 2N*
            // Answer = count(21*) + count(23*) * (N - 2)
            if (count_21star == 0 && nth_digit(can, N - 2) == 3)
                count_21star = ans;
            else if (nth_digit(can, N - 2) == 4) {
                ans = count_21star + (ans - count_21star) * (N - 2);
                return;
            }
        }

        auto ncache = cache;
        for (int c = 1; c < N; ++c)
            try_set_cache(ncache, nth_digit(can, N - 1 - c), c);

        const auto found = memo.find(ncache);
        if (found != memo.end())
            ans += found->second;
        else {
            rows[round] = can;
            const auto saved_ans = ans;
            inner_solve(round + 1, ncache);
            memo[ncache] = ans - saved_ans;
        }
    }
}

int main(int, char**)
{
    task_in >> N;

    rows.resize(N);
    required_rounds = N - 1;

    rows[0] = 1234567 / factor10[7 - N];

    uint64_t cache{};
    for (int d = 2; d <= N; ++d)
        try_set_cache(cache, d, d - 1);

    inner_solve(1, cache);

    task_out << ans * factorial[N - 1] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 latin.cpp -o latin.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address latin.cpp -o latin

Run:
latin.exe && type latin.out

Input:

7

Output:

12198297600

*/
