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

static const int factorial[] = {1, 1, 2, 6, 24, 120, 720};

int N, required_rounds;
int64_t count_21star, ans{};
std::vector<uint64_t> numcaches[10]; // MS digit -> {caches of numbers starting from MS digit}
std::unordered_map<uint64_t, int64_t> memo; // cache ignores left column

void update_cache(uint64_t& cache, const int digit, const int column)
{
    assert(0 < digit && digit < 8 && 1 <= column && column < 8);
    auto ptr = reinterpret_cast<uint8_t*>(&cache) + column - 1;
    *ptr |= 1 << (digit - 1);
}

void generate() {
    std::vector<int> digits(N);
    std::iota(digits.begin(), digits.end(), 1);
    do {
        const int first = digits.front();
        if (first > 1) { // ignore numbers starting with 1
            uint64_t cache{};
            for (int c = 1; c < N; ++c) {
                const int d = digits[c];
                update_cache(cache, d, c);
            }
            numcaches[first].push_back(cache);
        }
    } while (std::next_permutation(digits.begin(), digits.end()));
}

/**
 * Observation1: if we find acceptable latin square with sorted rows, we can have (N - 1)! variants of it by permuting all rows but the first one.
 * Observation2: Acceptable, row-sorted squares have left column fixed and equal to top row
 * Observation3: Correctly filled N-1 square can be extended to full N size in only one way
 * Observation4: We can cache answers for partially filled board
 * Observation5: Only cache value is needed, the filled rows are not.
 * Observation6: Computation for second row can by optimized, see coment inline below
 */
void inner_solve(const int round, const uint64_t cache)
{
    if (round == required_rounds) {
        ++ans;
        return;
    }

    for (const auto& can_cache : numcaches[round + 1]) {
        if (cache & can_cache)
            continue;
        if (round == 1) {
            // Observation6 : optimization for second row
            // 21* is fewer then 23* because for 21* digit 3 can not be anywhere in *, for 23* digit 1 can be anywhere
            // 23* and 24* are symmetric in any mean, the same hold true for 25* .. 2N*
            // Answer = count(21*) + count(23*) * (N - 2)
            if (count_21star == 0 && (static_cast<uint8_t>(can_cache) & 4))  // square[1][1] == 3
                count_21star = ans;
            else if (static_cast<uint8_t>(can_cache) & 8) { // square[1][1] == 4
                ans = count_21star + (ans - count_21star) * (N - 2);
                return;
            }
        }

        const auto ncache = cache | can_cache;
        const auto found = memo.find(ncache);
        if (found != memo.end())
            ans += found->second;
        else {
            const auto saved_ans = ans;
            inner_solve(round + 1, ncache);
            memo[ncache] = ans - saved_ans;
        }
    }
}

int main(int, char**)
{
    task_in >> N;

    generate();
    required_rounds = N - 1;

    uint64_t cache{};
    for (int d = 2; d <= N; ++d)
        update_cache(cache, d, d - 1);

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
