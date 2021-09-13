/*
ID: marcin.8
LANG: C++
TASK: pprime
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=LfpaKE1rATy&S=pprime
*/

#include <fstream>
#include <vector>

std::ifstream task_in("pprime.in");
std::ofstream task_out("pprime.out");

static int is_prime(const int64_t n) {
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int64_t i = 5; i * i <= n; ++i)
        if (n % i == 0)
            return false;
    return true;
}

static int numlen(const int64_t n) {
    int len{0}; 
    int64_t probe{1};
    while (probe < n) {
        probe *= 10;
        ++len;
    }
    return len;
}

static int numrev(int64_t n) {
    int64_t m{0};
    while (n) {
        m *= 10;
        m += n % 10;
        n /= 10;
    }
    return m;
}

int from, to;

static void finish(int n, const int shift, const int tail) {
    n *= 10 * shift;
    n += tail;
    if (from <= n && n <= to && is_prime(n))
        task_out << n << '\n';
}

int main(int, char**)
{
    task_in >> from >> to;

    for (int pp : {5, 7, 11}) // is_num_palindrome(n) && factorize(n).size() == 1
        if (from <= pp && pp <= to)
            task_out << pp << '\n';
    from = std::max(from, 101);

    const int nlt = numlen(to);
    for (int len = numlen(from); len <= nlt; ++len) {
        int ub{1}; // generator upper bound
        for (int z = len / 2 - 1; z; --z) // number of counter generated digits
            ub *= 10;
        for (const int d : {1, 3, 7, 9})
            for (int n = 0; n < ub; ++n) {
                const int candidate = d * ub + n;
                const int m = numrev(candidate);
                if (len & 1)
                    for (int mid = 0; mid < 10; ++mid)
                        finish(candidate * 10 + mid, ub, m);
                else
                    finish(candidate, ub, m);
            }
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 pprime.cpp -o pprime.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address pprime.cpp -o pprime

Input:


Output:


*/
