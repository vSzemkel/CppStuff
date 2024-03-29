/*
ID: marcin.8
LANG: C++17
TASK: theme
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=DMzbY6AZYHC&S=theme
*/

#include <algorithm>
#include <fstream>
#include <vector>

std::ifstream task_in("theme.in");
std::ofstream task_out("theme.out");

constexpr const int INF = 1e09;
constexpr const int MIN = 5;

static void solve() { // O(N2)
    int N, p;
    task_in >> N >> p; --N;
    std::vector<int> diffseq(N);
    for (auto& x : diffseq) {
        int n; task_in >> n;
        x = n - p;
        p = n;
    }

    int ans{0};
    for (int off = 1; off < N; ++off) {
        int cur{1};
        for (int i = 0; i < N - off; ++i)
            if (diffseq[i] == diffseq[i + off] && cur < off) // && do not overlap
                ++cur;
            else {
                ans = std::max(ans, cur);
                cur = 1;
            }
        ans = std::max(ans, cur);
    }

    if (ans < MIN)
        ans = 0;

    task_out << ans << '\n';
}

template <typename T, typename U>
static T last_true(T lo, T hi, U f) {
    lo--;
    while (lo < hi) { // find last index such that f is true
        const T mid = lo + (hi - lo + 1) / 2; // this will work for negative numbers too
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

/**
 * @brief Searches for prefixes of needle in the haystack string
 * 
 * @param s generalized string modelled as "needle$haystack"
 * @param n length of s
 * @return std::vector<int> ret[n] == k iff k-prefix of needle starts at haystack[n - 2 * size(needle)]
 */
template <typename T>
static std::vector<int> kmp(const T* s, const int n) {
    std::vector<int> ret(n);
    for (int k = 0, i = 1; i < n; ++i) { // k is lenght of already found proper (non identity) bound 
        while (k > 0 && s[k] != s[i])
            k = ret[k - 1];
        if (s[k] == s[i])
            ++k;
        ret[i] = k;
    }

    return ret;
}

/**
 * @brief Find maximal k that can be a solution to this problem using bisection search
 * 
 */
void solve_general() { // O(N2*LOG(N/2))
    int N, p;
    task_in >> N >> p; --N;
    std::vector<int> diffseq(N);
    for (auto& x : diffseq) {
        int n; task_in >> n;
        x = n - p;
        p = n;
    }

    const auto check = [&](const int k) -> bool {
        const auto ub = N - 2 * k;
        std::vector<int> nh = diffseq;
        nh.insert(nh.begin() + k, INF);
        auto beg = nh.data();
        for (int i = 0; i <= ub; ++i, ++beg) {
            const auto found = kmp(beg, N + 1 - i);
            if (std::find(found.begin(), found.end(), k) != found.end())
                return true;
            std::swap(beg[k], beg[k + 1]);
        }

        return false;
    };

    auto ans = last_true(0, N / 2, check);
    if (2 * ans < N)
        ++ans;
    if (ans < MIN)
        ans = 0;

    task_out << ans << '\n';
}

int main(int, char**)
{
    solve();
    // solve_general();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 theme.cpp -o theme.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address theme.cpp -o theme

Input:

30
25 27 30 34 39 45 52 60 69 79 69 60 52 45 39 34 30 26 22 18 82 78 74 70 66 67 64 60 65 80

Output:

5

*/
