
#include <cassert>
#include <iostream>
#include <vector>

// Duck, Duck, Geese
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008779b4/0000000000b45244


const int N = 1 << 19;

struct Node {
    int l, r;
    std::pair<int, int> val;
    int toAdd;

    Node() : l(), r(), val{-1, 0}, toAdd(0) {}
    Node(int _l, int _r) : l(_l), r(_r), val{0, _r - _l}, toAdd(0) {}

    void add(int x) {
        toAdd += x;
        val.first += x;
    }
};

Node tree[2 * N + 5];

void build() {
    for (int i = 0; i < N; i++)
        tree[N + i] = Node(i, i + 1);
    for (int i = N - 1; i > 0; i--)
        tree[i] = Node(tree[2 * i].l, tree[2 * i + 1].r);
}

void push(int v) {
    assert(v < N);
    if (tree[v].toAdd == 0) return;
    for (int u = 2 * v; u < 2 * v + 2; u++)
        tree[u].add(tree[v].toAdd);
    tree[v].toAdd = 0;
}

std::pair<int, int> merge(std::pair<int, int> v, std::pair<int, int> u) {
    if (v.first != u.first) return max(v, u);
    return {v.first, v.second + u.second};
}

void update(int v) {
    assert(v < N);
    tree[v].val = merge(tree[2 * v].val, tree[2 * v + 1].val);
}

void addOnSegm(int v, int l, int r, int x) {
    if (l <= tree[v].l && tree[v].r <= r) {
        tree[v].add(x);
        return;
    }
    if (l >= tree[v].r || tree[v].l >= r) return;
    push(v);
    for (int u = 2 * v; u < 2 * v + 2; u++)
        addOnSegm(u, l, r, x);
    update(v);
}

std::pair<int, int> getSegm(int v, int l, int r) {
    if (l <= tree[v].l && tree[v].r <= r) return tree[v].val;
    if (l >= tree[v].r || tree[v].l >= r) return {-1, 0};
    push(v);
    return merge(getSegm(2 * v, l, r), getSegm(2 * v + 1, l, r));
}

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

    const auto addOne = [&](int c, int l, int r, int del) {
        l = std::min(l, (int)pos[c].size() - 1);
        r = std::min(r, (int)pos[c].size() - 1);
        if (l >= r) return;
        l = pos[c][l] + 1;
        r = pos[c][r] + 1;
        addOnSegm(1, l, r, del);
    };

    const auto color = [&](int c, int del) {
        addOne(c, ptr[c], ptr[c] + 1, del);
        addOne(c, ptr[c] + lima[c], ptr[c] + limb[c] + 1, del);
    };

    build();
    for (int c = 0; c < C; ++c) {
        pos[c].push_back(2 * N);
        color(c, +1);
    }

    int64_t ans{0};
    for (int i = 0; i < N; ++i) {
        const auto cur = getSegm(1, i + 2, i + N);
        if (cur.first == C)
            ans += cur.second;
        const auto c = hats[i];
        color(c, -1);
        ++ptr[c];
        color(c, +1);
    }

    std::cout << ans;
}

static void solve_set1() { // some optimizations O(N*N); real 0m0.222s; MLE for Set2
    int N, C;
    std::cin >> N >> C;
    std::vector<int> lima(C), limb(C), hats(N);
    for (int i = 0; i < C; ++i)
        std::cin >> lima[i] >> limb[i];
    for (int i = 0; i < N; ++i) {
        std::cin >> hats[i];
        --hats[i];
    }

    std::vector<std::vector<int64_t>> prefsum(C, std::vector<int64_t>(2 * N + 1));
    for (int c = 0; c < C; ++c) {
        auto& pfs = prefsum[c];
        for (int i = 0; i < 2 * N; ++i) {
            pfs[i + 1] = pfs[i];
            if (hats[i % N] == c)
                ++pfs[i + 1];
        }
    }

    int64_t ans{0};
    for (int b = 0; b < N; ++b) {
        const int b_color = hats[b];
        if (limb[b_color] == 0)
            continue;
        int invalid_colors = (lima[b_color] <= 1) ? 0 : 1;
        for (int len = 2; len < N; ++len) {
            const int e = b + len - 1;
            const int c = hats[e % N];
            const auto& pref = prefsum[c];
            const int cnt = pref[e + 1] - pref[b];
            if (limb[c] < cnt)
                break;
            if (cnt == 1 && lima[c] > 0)
                ++invalid_colors;
            if (lima[c] == cnt)
                --invalid_colors;
            if (!invalid_colors)
                ++ans;
        }
    }

    std::cout << ans;
}

static void solve_set0() { // brute force O(N*N*C); real 0m10.122s
    int N, C;
    std::cin >> N >> C;
    std::vector<int> lima(C), limb(C), hats(N);
    for (int i = 0; i < C; ++i)
        std::cin >> lima[i] >> limb[i];
    for (int i = 0; i < N; ++i) {
        std::cin >> hats[i];
        --hats[i];
    }

    std::vector<std::vector<int64_t>> prefsum(C, std::vector<int64_t>(2 * N + 1));
    for (int c = 0; c < C; ++c) {
        auto& pfs = prefsum[c];
        for (int i = 0; i < 2 * N; ++i) {
            pfs[i + 1] = pfs[i];
            if (hats[i % N] == c)
                ++pfs[i + 1];
        }
    }

    int64_t ans{0};
    for (int b = 0; b < N; ++b)
        for (int len = 2; len < N; ++len) {
            bool ok{true};
            const int e = b + len - 1;
            for (int c = 0; ok && c < C; ++c) {
                const int cnt = prefsum[c][e + 1] - prefsum[c][b];
                ok = !cnt || (lima[c] <= cnt && cnt <= limb[c]);
            }
            if (ok)
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

Case #1: 2
Case #2: 9
Case #3: 1

*/
