
#include <iostream>
#include <vector>

// Building Palindromes
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050eda/0000000000119866

constexpr int pct(int x) { return __builtin_popcount(x); } // # of bits set

template <typename T = int>
struct simple_segment_tree_t
{
  public:
    simple_segment_tree_t(const std::vector<T>& input) {
        const int input_size = (int)input.size();
        while (_offset < input_size) _offset *= 2;
        _nodes.assign(2 * _offset, SEED);
        std::copy(input.begin(), input.end(), _nodes.begin() + _offset);
        for (int n = _offset - 1; n > 0; --n)
            _nodes[n] = join(_nodes[2 * n], _nodes[2 * n + 1]);
    }

    T query(int lower, int upper) const { // range [lower, upper)
        T ret{SEED};
        for (lower += _offset, upper += _offset; lower < upper; lower >>= 1, upper >>= 1) { 
            if (lower & 1)
                ret = join(ret, _nodes[lower++]); 
            if (upper & 1)
                ret = join(ret, _nodes[--upper]);
        }

        return ret;
    }

    void update(int pos, const T val) {
        pos += _offset;
        _nodes[pos] = val;
        while (pos > 1) {
            pos /= 2;
            _nodes[pos] = join(_nodes[2 * pos], _nodes[2 * pos + 1]);
        }
    }

    static const T SEED{0}; // TO DO: change value accordingly

  private:
    static T join(const T& lhs, const T& rhs) {
        return lhs ^ rhs; // TO DO: change join implementation accordingly
    }

    std::vector<T> _nodes;
    int _offset{1};
};

static void solve() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> data(N);
    for (auto& d : data) {
        char c; std::cin >> c;
        d = 1 << (c - 'A');
    }

    simple_segment_tree_t st{data};
    int ans{0};
    for (int q = 0; q < Q; ++q) {
        int b, e;
        std::cin >> b >> e;
        if (pct(st.query(--b, e)) < 2)
            ++ans;
    }

    std::cout << ans;
}

static void solve_set1() { // MLE
    int N, Q;
    std::cin >> N >> Q;
    std::string letters;
    std::cin >> letters;

    std::vector<std::vector<int>> dp(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        dp[i][i] = 1 << (letters[i] - 'A');
    for (int len = 2; len <= N; ++len)
        for (int last = len - 1; last < N; ++last)
            dp[last - len + 1][last] = dp[last - len + 1][last - 1] ^ (1 << (letters[last] - 'A'));

    int ans{0};
    for (int q = 0; q < Q; ++q) {
        int b, e;
        std::cin >> b >> e;
        if (pct(dp[--b][--e]) < 2)
            ++ans;
    }

    std::cout << ans;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 building_palindromes.cpp -o building_palindromes.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address building_palindromes.cpp -o building_palindromes

Run:
building_palindromes.exe < building_palindromes.in

Input:

2
7 5
ABAACCA
3 6
4 4
2 5
6 7
3 7
3 5
XYZ
1 3
1 3
1 3
1 3
1 3

Output:


*/
