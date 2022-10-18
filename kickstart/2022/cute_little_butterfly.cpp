
#include <algorithm>
#include <array>
#include <iostream>
#include <functional>
#include <map>
#include <utility>
#include <vector>

// Cute Little Butterfly
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb2e1/0000000000c17b68

template <typename T = int64_t>
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

    static inline const T SEED{std::numeric_limits<int>::min()}; // TO DO: change value accordingly

  private:
    static inline T join(const T& lhs, const T& rhs) {
        return std::max(lhs, rhs);
    }

    std::vector<T> _nodes;
    int _offset{1};
};

constexpr const int MAXN = 100004;

static void solve() { // by IBory
    int64_t N, E;
    std::cin >> N >> E;
    std::map<int, std::vector<std::pair<int, int>>, std::greater<>> flowers; // altitude -> (distance, power)
    for (int i = 0; i < N; ++i) {
        int x, y, p;
        std::cin >> x >> y >> p;
        flowers[y].emplace_back(x, p);
    }

    // for every distance st_left stores maximal score lowering the flight so far and facing left
    simple_segment_tree_t st_right{std::vector<int64_t>(MAXN, 0)};
    simple_segment_tree_t st_left{std::vector<int64_t>(MAXN, -E)};
    for (auto& [y, f] : flowers) {
        std::sort(f.begin(), f.end());
        std::map<int, int64_t> score_left, score_right;
        // move down
        for (const auto& [x, p] : f) {
            std::array<int64_t, 3> canl, canr;
            canr[0] = st_right.query(0, x + 1) + p;
            canr[1] = st_right.query(x, MAXN) + p - 2 * E;
            canr[2] = st_left.query(0, MAXN) + p - E;
            score_right[x] = *std::max_element(canr.begin(), canr.end());
            canl[0] = st_left.query(x, MAXN) + p;
            canl[1] = st_left.query(0, x + 1) + p - 2 * E;
            canl[2] = st_right.query(0, MAXN) + p - E;
            score_left[x] = *std::max_element(canl.begin(), canl.end());
        }
        // move horizontaly
        const auto hor = int(f.size());
        for (int i = 1; i < hor; ++i) {
            const auto& [x, p] = f[i];
            score_right[x] = std::max(score_right[x], score_right[f[i - 1].first] + p);
        }
        for (int i = hor - 2; i >= 0; --i) {
            const auto& [x, p] = f[i];
            score_left[x] = std::max(score_left[x], score_left[f[i + 1].first] + p);
        }
        // update stats
        for (const auto& [x, s] : score_right)
            st_right.update(x, s);
        for (const auto& [x, s] : score_left)
            st_left.update(x, s);
    }

    std::cout << std::max(st_left.query(0, MAXN), st_right.query(0, MAXN));
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 cute_little_butterfly.cpp -o cute_little_butterfly.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address cute_little_butterfly.cpp -o cute_little_butterfly

Run:
cute_little_butterfly.exe < cute_little_butterfly.in

Input:

2
4 10
1 1 2
1 2 2
2 1 2
2 2 2
6 5
1 1 4
1 3 1
3 4 5
4 3 2
5 2 1
3 2 10

Output:

Case #1: 6
Case #2: 17

*/
