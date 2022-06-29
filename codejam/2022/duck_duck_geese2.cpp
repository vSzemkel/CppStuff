
#include <iostream>
#include <vector>

// Duck, Duck, Geese
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008779b4/0000000000b45244


template <typename T = std::pair<int, int>> // {number of colors ending here, number of ranges ending here}
struct updatable_segment_tree_t
{
  public:
    updatable_segment_tree_t(const int size) : _size(size) {
        _offset = 1;
        while (_offset < size)
            _offset <<= 1;
        _changes.assign(2 * _offset, 0);
        _nodes.assign(2 * _offset, ZERO);
        _build();
    }

    T query(const int lower, const int upper) { // range [lower, upper)
        return _query(lower, upper, 1, 0, _offset);
    }

    void update(const int lower, const int upper, const int delta) {
        _update(lower, upper, delta, 1, 0, _offset);
    }

    static inline const T SEED{0, 1}; // TO DO: change value accordingly
    static inline const T ZERO{-1, 0}; // TO DO: change value accordingly

  private:
    static T join(const T& lhs, const T& rhs) {
        if (lhs.first != rhs.first) return std::max(lhs, rhs);
        return {lhs.first, lhs.second + rhs.second};
    }

    inline void upd(const int id, const int delta) {
        _changes[id] += delta;
        _nodes[id].first += delta;
    }

    inline void push(const int id, const int left, const int right) {
        if (_changes[id] == 0)
            return;
        upd(left, _changes[id]);
        upd(right, _changes[id]);
        _changes[id] = 0;
    }

    void _build() {
        for (int i = 0; i < _offset; ++i)
            _nodes[_offset + i] = SEED;
        for (int i = _offset - 1; ~i; --i)
            _nodes[i] = join(_nodes[2 * i], _nodes[2 * i + 1]);
    }

    T _query(const int lower, const int upper, const int root, const int l, const int r) { // range [lower, upper)
        if (lower >= r || upper <= l)
            return ZERO;
        if (lower <= l && r <= upper)
            return _nodes[root];

        const int mid = l + ((r - l) >> 1), il = root << 1, ir = il + 1;
        push(root, il, ir);
        return join(_query(lower, upper, il, l, mid), _query(lower, upper, ir, mid, r));
    }

    void _update(const int lower, const int upper, const int delta, const int root, const int l, const int r) {
        if (lower >= r || upper <= l)
            return;
        if (lower <= l && r <= upper) {
            upd(root, delta);
            return;
        }
        const int mid = l + ((r - l) >> 1), il = root << 1, ir = il | 1;
        push(root, il, ir);
        _update(lower, upper, delta, il, l, mid);
        _update(lower, upper, delta, ir, mid, r);
        _nodes[root] = join(_nodes[il], _nodes[ir]);
    }

    int _size, _offset;
    std::vector<int> _changes;
    std::vector<T> _nodes;
};

static void solve() { // some optimizations O(N*LOGN); real 0m0.222s; by Um_nik
    int N, C;
    std::cin >> N >> C;
    std::vector<int> lima(C), limb(C), hats(N);
    for (int i = 0; i < C; ++i) {
        std::cin >> lima[i] >> limb[i];
        if (lima[i] == 0) ++lima[i];
    }
    for (int i = 0; i < N; ++i) {
        std::cin >> hats[i];
        --hats[i];
    }

    std::vector<int> ptr(C); // next position of color to consider
    std::vector<std::vector<int>> pos(C, {-1}); // positions of color
    for (int i = 0; i < 2 * N; ++i)
        pos[hats[i % N]].push_back(i);

    updatable_segment_tree_t st{2*N};
    const auto addOne = [&](int c, int l, int r, int del) {
        l = std::min(l, (int)pos[c].size() - 1);
        r = std::min(r, (int)pos[c].size() - 1);
        if (l >= r) return;
        l = pos[c][l] + 1;
        r = pos[c][r] + 1;
        st.update(l, r, del);
    };

    const auto color = [&](int c, int del) {
        addOne(c, ptr[c], ptr[c] + 1, del);
        addOne(c, ptr[c] + lima[c], ptr[c] + limb[c] + 1, del);
    };

    for (int c = 0; c < C; ++c) {
        pos[c].push_back(2 * N);
        color(c, +1);
    }

    int64_t ans{0};
    for (int i = 0; i < N; ++i) {
        const auto cur = st.query(i + 2, i + N);
        if (cur.first == C)
            ans += cur.second;
        const auto c = hats[i];
        color(c, -1);
        ++ptr[c];
        color(c, +1);
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 duck_duck_geese.cpp -o duck_duck_geese.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address duck_duck_geese.cpp -o duck_duck_geese

Run:
duck_duck_geese.exe < duck_duck_geese.in

Input:

3
3 2
1 1
1 1
1 1 2
5 2
1 1
1 2
1 2 1 2 2
3 3
1 2
1 2
2 2
1 1 3

Output:


*/
