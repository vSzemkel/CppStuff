
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// X or What
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051061/0000000000161426

static int score(const int x) {
    return (__builtin_popcount(x) & 1) ? 0 : 1;
}

template <typename T>
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

    static inline const T SEED{-1}; // TO DO: change value accordingly

  private:
    static inline T join(const T& lhs, const T& rhs) {
        if (lhs < 0)
            return rhs;
        if (rhs < 0)
            return lhs;

        return lhs ^ rhs;
    }

    std::vector<T> _nodes;
    int _offset{1};
};

static void solve() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> input(N);
    for (auto& n : input)
        std::cin >> n;

    simple_segment_tree_t<int> st{input};
    for (int z = Q; z; --z) {
        int pos, val;
        std::cin >> pos >> val;
        st.update(pos, val);

        bool found{};
        for (int len = N; len && !found; --len)
            for (int begin = 0; begin + len  <= N; ++begin)
                if (score(st.query(begin, begin + len))) {
                    found = true;
                    std::cout << len << ' ';
                    break;
                }
        
        if (!found)
            std::cout << "0 ";
    }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 x_or_what.cpp -o x_or_what.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address x_or_what.cpp -o x_or_what

Run:
x_or_what.exe < x_or_what.in

Input:

2
4 3
10 21 3 7
1 13
0 32
2 22
5 1
14 1 15 20 26
4 26

Output:


*/
