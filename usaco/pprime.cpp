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

static std::vector<int64_t> factorize(int64_t n) {
    std::vector<int64_t> factorization;
    if (n < 1) return factorization;

    for (int d : {2, 3, 5}) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }

    int i{0}, increments[] = {6, 4, 2, 4, 2, 4, 6, 2};
    for (int64_t d = 7; d * d <= n; d += increments[i]) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
        i = (i + 1) % 8;
    }

    if (n > 1 || factorization.empty())
        factorization.push_back(n);

    return factorization;
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
    if (from <= n && n <= to && factorize(n).size() == 1)
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
