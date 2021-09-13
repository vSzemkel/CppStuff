/*
ID: marcin.8
LANG: C++
TASK: crypt1
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=XoDpSeUFv1V&S=crypt1
*/

#include <fstream>
#include <vector>

// For N > 6 better solution is to enumerate all possible numbers, e.g.
//  for (i=100; i<1000; ++i)
//    for (j=10; j<100; ++j)

std::ifstream task_in("crypt1.in");
std::ofstream task_out("crypt1.out");

std::vector<bool> present(10);

static bool is_compliant(int n)
{
    while (n > 0) {
        if (!present[n % 10]) return false;
        n /= 10;
    }
    return true;
}

int main(int, char**)
{
    int N; task_in >> N;
    std::vector<int> digits(N);
    for (int i = 0; i < N; ++i) {
        task_in >> digits[i];
        present[digits[i]] = true;
    }

    int64_t ret{0};
    for (const int d1 : digits)
        for (const int d2 : digits)
            for (const int d3 : digits) // d3d2d1
                for (const int d4 : digits)
                    for (const int d5 : digits) { // d5d4
                        const int first = d3 * 100 + d2 * 10 + d1;
                        const int partial1 = first * d4;
                        if (partial1 > 999 || !is_compliant(partial1)) continue;
                        const int partial2 = first * d5;
                        if (partial2 > 999 || !is_compliant(partial2)) continue;
                        const int result = 10 * partial2 + partial1;
                        if (result <= 9999 && is_compliant(result))
                            ++ret;
                    }

    task_out << ret << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 crypt1.cpp -o crypt1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 crypt1.cpp -o crypt1.o

Input:

5
2 3 4 6 8

Output:

GO

*/
