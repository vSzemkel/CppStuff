/*
ID: marcin.8
LANG: C++
TASK: sprime
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=czIYf0BMp0z&S=sprime
*/

#include <fstream>
#include <vector>

std::ifstream task_in("sprime.in");
std::ofstream task_out("sprime.out");

static int is_prime(const int64_t n) {
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

int main(int, char**)
{
    int L; task_in >> L;
    std::vector<std::vector<int>> superprimes(L);
    superprimes[0] = {2, 3, 5, 7};
    for (int len = 1; len < L; ++len) {
        auto& cur = superprimes[len];
        for (const int sp : superprimes[len - 1])
            for (const int last : {1, 3, 7, 9}) {
                const int candidate = sp * 10 + last;
                if (is_prime(candidate))
                    cur.push_back(candidate);
            }
    }

    for (const int sp : superprimes.back())
        task_out << sp << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 sprime.cpp -o sprime.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address sprime.cpp -o sprime

Input:

8

Output:

23399339
29399999
37337999
59393339
73939133

*/
