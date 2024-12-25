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
static const int factor10[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

int N, required_rounds;
int64_t count_21star, ans{};
std::vector<int> rows;
std::unordered_map<uint64_t, int64_t> memo; // cache ignores left column and numbers starting with 1
std::unordered_map<int, std::vector<std::pair<int, uint64_t>>> numbers; // MS digit -> {number, cache}

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
        if (first > 1) {
            int num{first};
            uint64_t cache{};
            for (int c = 1; c < N; ++c) {
                const int d = digits[c];
                num = 10 * num + d;
                update_cache(cache, d, c);
            }
            numbers[first].emplace_back(num, cache);
        }
    } while (std::next_permutation(digits.begin(), digits.end()));
}

/**
 * Observation1: if we find acceptable latin square with sorted rows, we can have (N - 1)! variants of it by permuting all rows but the first one.
 * Observation2: Acceptable, row-sorted squares have left column fixed and equal to top row
 * Observation3: Correctly filled N-1 square can be extended to full N size in only one way
 * Observation4: We can cache answers for partially filled board
 * Observation5: Computation for second row can by optimized, see coment inline below
 */
void inner_solve(const int round, uint64_t cache)
{
    if (round == required_rounds) {
        ++ans;
        return;
    }

    for (const auto& [can, can_cache] : numbers[round + 1]) {
        if ((cache & can_cache) != 0)
            continue;
        if (round == 1) {
            const auto nth_digit = [](int number, int n) { return number / factor10[n] % 10; };
            // Observation5 : optimization for second row
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

        const auto ncache = cache | can_cache;
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

    generate();
    rows.resize(N);
    required_rounds = N - 1;

    rows[0] = 1234567 / factor10[7 - N];

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
