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
static std::vector<board_t> solution;
static std::vector<std::string> tails;
static std::unordered_set<int> all;
static std::vector<std::string> primes[10][10];
static std::string_view top, bottom, left, right, middle[5];

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
            all.insert(i);
            auto s = std::to_string(i);
            primes[s[0] - '0'][s[4] - '0'].push_back(s);
            if (p(s[0]) && p(s[1]) && p(s[2]) && p(s[3]) && p(s[4]))
                tails.push_back(s);
        }
}

bool check(const int p) {
    return all.count(p);
};

int get_col(int c) {
    int ret = top[c] - '0';
    for (int r = 1; r <= 3; ++r)
        ret = 10 * ret + middle[r][c] - '0';
    return 10 * ret + bottom[c] - '0';
}

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

    const std::function<void(int)> inner_solve = [&](const int r)
    {
        if (r == 4) {
            if (check(get_col(1)) && check(get_col(2)) && check(get_col(3))) {
                int diag = top[0] - '0';
                diag = 10 * diag + middle[1][1] - '0';
                diag = 10 * diag + middle[2][2] - '0';
                diag = 10 * diag + middle[3][3] - '0';
                if (check(10 * diag + bottom[4]  - '0')) {
                    diag = bottom[0]  - '0';
                    diag = 10 * diag + middle[3][1] - '0';
                    diag = 10 * diag + middle[2][2] - '0';
                    diag = 10 * diag + middle[1][3] - '0';
                    if (check(10 * diag + top[4]  - '0')) {
                        board[0] = top;
                        board[1] = middle[1];
                        board[2] = middle[2];
                        board[3] = middle[3];
                        board[1][0] = left[1]; board[1][4] = right[1];
                        board[2][0] = left[2]; board[2][4] = right[2];
                        board[3][0] = left[3]; board[3][4] = right[3];
                        board[4] = bottom;
                        solution.push_back(board);
                    }
                }
            }
        } else
            for (const auto& row : primes[left[r] - '0'][right[r] - '0']) {
                middle[r] = row;
                inner_solve(r + 1);
            }
    };

    std::vector<std::string_view> inits[10];
    const auto& starting_from_n = primes[N];
    for (int tr : {1, 3, 7, 9}) {
        std::copy_if(starting_from_n[tr].begin(), starting_from_n[tr].end(), std::back_inserter(inits[tr]), [](const auto sv){
            return sv[1] != '0' && sv[2] != '0' && sv[3] != '0' && sv[4] != '0';
        });

        if (inits[tr].empty())
            tails.erase(std::remove_if(tails.begin(), tails.end(), [tr](const auto sv){
                return sv[0] == tr + '0';
            }), tails.end());
    }

    board.fill(std::string(5, ' '));
    for (int tr : {1, 3, 7, 9})
        for (const auto& t : inits[tr]) {
            top = t;
            for (const auto& r : tails)
                if (r[0] == tr + '0') {
                    right = r;
                    for (const auto& b : tails)
                        if (b[4] == r[4]) {
                            bottom = b;
                            for (const auto& l : inits[b[0] - '0'])
                                if (!primes[l[1] - '0'][r[1] - '0'].empty() && !primes[l[2] - '0'][r[2] - '0'].empty() && !primes[l[3] - '0'][r[3] - '0'].empty()
                                 && !primes[t[0] - '0'][b[4] - '0'].empty() && !primes[b[0] - '0'][t[4] - '0'].empty()) {
                                    left = l;
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
