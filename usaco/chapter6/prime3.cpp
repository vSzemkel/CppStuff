/*
ID: marcin.8
LANG: C++
TASK: prime3
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=p77QAdyHaX0&S=prime3
*/

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

using board_t = std::array<std::string, 5>;
static board_t board;
static auto& r0 = board[0];
static auto& r1 = board[1];
static auto& r2 = board[2];
static auto& r3 = board[3];
static auto& r4 = board[4];
static std::vector<board_t> solution;
static std::vector<std::string_view> tails;
static std::unordered_set<std::string> all;
static std::vector<std::string_view> primes[58][58];

static void generate(const int64_t lower, const int64_t upper, const int sum) {
    std::vector<bool> taken(upper);
    const auto sqrt = (int)std::sqrt(upper);
    const auto p = [](const char c) { return c == '1' || c == '3' || c == '7' || c == '9'; };
    const auto ds = [](int x) { int ret{}; while (x) { ret += x % 10; x /= 10; } return ret; };

    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int j = i * i; j < upper; j += i)
                taken[j] = true;

    for (int i = 2; i < upper; ++i)
        if (!taken[i] && lower <= i && ds(i) == sum) {
            const auto [s, b] = all.insert(std::to_string(i));
            std::string_view sv{*s};
            if (p(sv[0]) && p(sv[1]) && p(sv[2]) && p(sv[3]) && p(sv[4]))
                tails.push_back(sv);
            primes[sv[0]][sv[4]].push_back(sv);
        }
}

bool check(const std::string& s) {
    return all.count(s);
};

void print() {
    if (solution.empty())
        task_out << "NONE\n";
    else {
        const int sz = int(solution.size());
        std::sort(solution.begin(), solution.end());
        for (int i = 0; i < sz - 1; ++i) {
            const auto& b = solution[i];
            for (int r = 0; r < 5; ++r)
                task_out << b[r] << '\n';
            task_out << '\n';
        }
        const auto& b = solution.back();
        for (int r = 0; r < 5; ++r)
            task_out << b[r] << '\n';
    }
}

/**
 * Observation: Top row number must not contain 0 digit
 * Observation: Left column number must not contain 0 digit
 * Observation: Right and bottom numbers can contain only specific digits
 */
int main(int, char**)
{
    int S, N;
    task_in >> S >> N;

    generate(10000, 100000, S);

    const std::function<void(int)> inner_solve = [&](const int c)
    {
        if (c == 4) {
            if (check(r1) && check(r2) && check(r3)) {
                std::string diag{r0};
                diag[1] = r1[1];
                diag[2] = r2[2];
                diag[3] = r3[3];
                diag[4] = r4[4];
                if (check(diag)) {
                    diag[0] = r4[0];
                    diag[1] = r3[1];
                    diag[3] = r1[3];
                    diag[4] = r0[4];
                    if (check(diag))
                        solution.push_back(board);
                }
            }
        } else
            for (const auto& row : primes[r0[c]][r4[c]]) {
                r1[c] = row[1];
                r2[c] = row[2];
                r3[c] = row[3];
                inner_solve(c + 1);
            }
    };

    board.fill(std::string(5, ' '));
    const auto& inits = primes['0' + N];
    for (char tr : {'1', '3', '7', '9'})
        for (const auto& top : inits[tr])
            if (top.find('0', 1) == std::string::npos) {
                r0 = top;
                for (const auto& right : tails)
                    if (right[0] == tr) {
                        r1[4] = right[1];
                        r2[4] = right[2];
                        r3[4] = right[3];
                        for (const auto& bottom : tails)
                            if (bottom[4] == right[4]) {
                                r4 = bottom;
                                for (const auto& left : inits[bottom[0]])
                                    if (left.find('0', 1) == std::string::npos
                                    && !primes[left[1]][right[1]].empty() && !primes[left[2]][right[2]].empty() && !primes[left[3]][right[3]].empty()
                                    && !primes[r0[0]][r4[4]].empty() && !primes[r4[0]][r0[4]].empty()) {
                                        r1[0] = left[1];
                                        r2[0] = left[2];
                                        r3[0] = left[3];
                                        inner_solve(1);
                                    }
                            }
                    }
            }

    print();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 prime3.cpp -o prime3.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address prime3.cpp -o prime3

Run:
prime3.exe && type prime3.out

Input:

17 7

Output:

71153
13553
15137
55331
33713

and 3 more

*/
