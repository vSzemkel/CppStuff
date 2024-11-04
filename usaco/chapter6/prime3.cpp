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
#include <unordered_set>
#include <utility>
#include <vector>

std::ifstream task_in("prime3.in");
std::ofstream task_out("prime3.out");

static std::pair<std::array<std::vector<int>, 10>, std::array<std::vector<std::string>, 10>> generate(const int64_t lower, const int64_t upper, const int sum) { // generate primes in [range)
    std::vector<bool> taken(upper);
    const auto sqrt = (int)std::sqrt(upper);
    for (int i = 2; i <= sqrt; ++i)
        if (!taken[i])
            for (int j = i * i; j < upper; j += i)
                taken[j] = true;

    std::array<std::vector<int>, 10> iprimes;
    std::array<std::vector<std::string>, 10> sprimes;
    for (int i = 2; i < upper; ++i)
        if (!taken[i] && lower <= i) {
            int s{}, c{i};
            while (10 <= c) {
                s += (c % 10);
                c /= 10;
            }
            if (c + s == sum) {
                iprimes[c].push_back(i);
                sprimes[c].push_back(std::to_string(i));
            }
        }

    return {iprimes, sprimes};
}

static std::array<std::string, 5> board;
static std::array<std::vector<int>, 10> iprimes;
static std::array<std::vector<std::string>, 10> sprimes;
static const std::unordered_set<int> invalid = {79, 89, 96, 98, 99};

bool check(const std::string& s) {
    const auto& bucket = sprimes[s.front() - '0'];
    return std::binary_search(bucket.begin(), bucket.end(), s);
};

bool prime_tail(const char c) {
    return c == '1' || c == '3' || c == '7' || c == '9';
};

/**
 * Observation: Top row number must not contain 0 digit
 * Observation: Left column number must not contain 0 digit
 * Observation: There are no 5 digits primes starting from some 2 digits prefix
 */
int main(int, char**)
{
    int S, N;
    task_in >> S >> N;

    bool found{};
    std::tie(iprimes, sprimes) = generate(10000, 100000, S);

    const std::function<void(int, const std::string&, const std::string&)> inner_solve = [&](const int c, const std::string& top, const std::string& r1)
    {
        if (c < 0) {
            if (check(board[2]) && check(board[3]) && check(board[4])) {
                std::string diag;
                diag.resize(5);
                diag[0] = N + '0';
                diag[1] = r1[1];
                diag[2] = board[2][2];
                diag[3] = board[3][3];
                diag[4] = board[4][4];
                if (check(diag)) {
                    diag[0] = board[4][0];
                    diag[1] = board[3][1];
                    diag[3] = r1[3];
                    diag[4] = top.back();
                    if (check(diag)) {
                        if (found)
                            task_out << '\n';
                        else
                            found = true;
                        task_out << top << '\n' << r1 << '\n';
                        for (int r = 2; r < 5; ++r)
                            task_out << board[r] << '\n';
                    }
                }
            }
        } else {
            const int bucket = top[c] - '0';
            const int low = 1000 * (10 * bucket + r1[c] - '0');
            const int hi = low + 1000;
            const auto& cont = iprimes[bucket];
            for (auto it = std::lower_bound(cont.begin(), cont.end(), low); it != cont.end() && *it < hi; ++it) {
                const auto& selected = sprimes[bucket][it - cont.begin()];
                if (prime_tail(selected.back())) {
                    board[2][c] = selected[2];
                    board[3][c] = selected[3];
                    board[4][c] = selected[4];
                    inner_solve(c - 1, top, r1);
                }
            }
        }
    };

    board.fill(std::string(5, ' '));
    for (const auto& top : sprimes[N])
        if (top.find('0', 1) == std::string::npos) {
            const int t = top.back() - '0';
            for (const auto& right : sprimes[t])
                if (prime_tail(right[1]) && prime_tail(right[2]) && prime_tail(right[3]) && prime_tail(right[4]))
                    for (int i = 1; i < 10; ++i)
                        for (const auto& r1 : sprimes[i]) {
                            if (r1.back() == right[1]) {
                                if (invalid.count(10 * (top[0] - '0') + r1[0] - '0')
                                || invalid.count(10 * (top[1] - '0') + r1[1] - '0')
                                || invalid.count(10 * (top[2] - '0') + r1[2] - '0')
                                || invalid.count(10 * (top[3] - '0') + r1[3] - '0'))
                                    break;
                                board[2][4] = right[2];
                                board[3][4] = right[3];
                                board[4][4] = right[4];
                                inner_solve(3, top, r1);
                            }
                        }
        }

    if (!found)
        task_out << "NONE\n";
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
