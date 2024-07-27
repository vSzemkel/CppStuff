/*
ID: marcin.8
LANG: C++
TASK: hidden
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=D2EK5jk34oV&S=hidden
*/

#include <fstream>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

std::ifstream task_in("hidden.in");
std::ofstream task_out("hidden.out");

struct suffix_array_t {
    static auto sort_cyclic_shifts(const std::string& s) { // O(NlogN)
        const int alphabet = 256;
        const int n = int(s.size());
        // preprocess single characters with counting sort
        std::vector<int> rank(n), cls(n), counts(std::max(alphabet, n));
        for (int i = 0; i < n; ++i)
            ++counts[s[i]];
        std::partial_sum(counts.cbegin(), counts.cend(), counts.begin());
        for (int i = 0; i < n; ++i)
            rank[--counts[s[i]]] = i;

        int classes{1};
        cls[rank[0]] = 0;
        for (int i = 1; i < n; ++i) {
            if (s[rank[i]] != s[rank[i - 1]])
                ++classes;
            cls[rank[i]] = classes - 1;
        }

        std::vector<int> rn(n), cn(n);
        for (int h = 0; (1 << h) < n; ++h) {
            for (int i = 0; i < n; ++i) {
                rn[i] = rank[i] - (1 << h);
                if (rn[i] < 0)
                    rn[i] += n;
            }

            std::fill(counts.begin(), counts.begin() + classes, 0);
            for (int i = 0; i < n; ++i)
                ++counts[cls[rn[i]]];
            std::partial_sum(counts.cbegin(), counts.cbegin() + classes, counts.begin());
            for (int i = n - 1; ~i; --i)
                rank[--counts[cls[rn[i]]]] = rn[i];

            classes = 1;
            cn[rank[0]] = 0;
            for (int i = 1; i < n; ++i) {
                const std::pair<int, int> prev = {cls[rank[i - 1]], cls[(rank[i - 1] + (1 << h)) % n]};
                const std::pair<int, int> cur = {cls[rank[i]], cls[(rank[i] + (1 << h)) % n]};
                if (cur != prev)
                    ++classes;
                cn[rank[i]] = classes - 1;
            }

            cls.swap(cn);
        }

        return rank;
    }

    static std::vector<int> suffix_array(const std::string& s) {
        const auto sorted_shifts = sort_cyclic_shifts(s + '$');
        return {sorted_shifts.begin() + 1, sorted_shifts.end()};
    }
};

int main(int, char**)
{
    int L, sz{};
    task_in >> L;
    std::stringstream sm;
    while (sz < L) {
        std::string chunk;
        task_in >> chunk;
        sm << chunk;
        sz += chunk.size();
    }

    std::string text = sm.str() + sm.str();
    const auto order = suffix_array_t::suffix_array(text);
    auto o = order.data();
    while (*o > L)
        ++o;
    // for uniform texts answer will be L and we should return 0
    task_out << (*o % L) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 hidden.cpp -o hidden.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address hidden.cpp -o hidden

Run:
hidden.exe && type hidden.out

Input:

7
alabala

Output:

6

*/
