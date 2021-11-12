/*
ID: marcin.8
LANG: C++
TASK: preface
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=PgGkyrEPgRl&S=preface
*/

#include <array>
#include <fstream>

std::ifstream task_in("preface.in");
std::ofstream task_out("preface.out");

constexpr char digits[] = "IVXLCDM--";

static int count_digits(int n) // ABCDE -> 5
{
    int d{1};
    while (true) {
      n /= 10;
      if (n == 0) break;
      ++d;
    }

    return d;
}

static int max_factor(const int n) // ABCDE -> 10000
{
    int ret{1};
    while (10 * ret <= n)
        ret *= 10;
    return ret;
}

int main(int, char**)
{
    std::array<int, 3> code[10];
    code[0] = {0, 0, 0};
    code[1] = {1, 0, 0};
    code[2] = {2, 0, 0};
    code[3] = {3, 0, 0};
    code[4] = {1, 1, 0};
    code[5] = {0, 1, 0};
    code[6] = {1, 1, 0};
    code[7] = {2, 1, 0};
    code[8] = {3, 1, 0};
    code[9] = {1, 0, 1};

    std::array<int, 3> cumu[10];
    cumu[0] = code[0];
    for (int i = 1; i < 10; ++i) {
        cumu[i][0] = cumu[i - 1][0] + code[i][0];
        cumu[i][1] = cumu[i - 1][1] + code[i][1];
        cumu[i][2] = cumu[i - 1][2] + code[i][2];
    }

    int N; task_in >> N;

    std::array<int, 9> ans{}; // {#I, #V, .., #D, #M, #-, #-}
    for (int fac = max_factor(N), pos = 2 * (count_digits(fac) - 1); fac > 0; fac /= 10, pos -= 2) {
        const int full = (N / fac) % 10;
        if (full > 0) {
            ans[pos]     += cumu[full - 1][0] * fac;
            ans[pos + 1] += cumu[full - 1][1] * fac;
            ans[pos + 2] += cumu[full - 1][2] * fac;
            const int tail = (N % fac) + 1;
            ans[pos]     += code[full][0] * tail;
            ans[pos + 1] += code[full][1] * tail;
            ans[pos + 2] += code[full][2] * tail;

            const int f = fac * full / 10;
            for (int p = pos - 2; p >= 0; p -= 2) {
                ans[p]     += cumu[9][0] * f;
                ans[p + 1] += cumu[9][1] * f;
                ans[p + 2] += cumu[9][2] * f;
            }
        }
    }

    for (int pos = 0; pos < 7; ++pos)
        if (ans[pos] > 0)
            task_out << digits[pos] << ' ' << ans[pos] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 preface.cpp -o preface.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address preface.cpp -o preface

Input:

2974

Output:

I 4165
V 1486
X 4447
L 1485
C 4465
D 1500
M 3225

*/
