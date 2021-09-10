
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <utility>
#include <vector>

// Slide circuits
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000436329/000000000084f7b2


int B, S, N;
std::vector<std::pair<int, int>> slides;

static void solve() { // by tourist & ecnerwala
    std::cin >> B >> S >> N;
    // assign randoms to buildings
    uint64_t total{0};
    std::mt19937_64 rng{};
    std::vector<uint64_t> bids(B);
    for (auto& b : bids) {
        b = rng();
        total ^= b;
    }
    // build initial state
    slides.resize(S);
    std::map<std::pair<uint64_t, uint64_t>, int> slide_inverse_ids;
    for (int i = 0; i < S; ++i) {
        int from, to; std::cin >> from >> to;
        --from, --to;
        slides[i] = {from, to};
        slide_inverse_ids[{total ^ bids[from], total ^ bids[to]}] = i;
    }

    std::vector<uint64_t> aux(B);
    for (int i = 0; i < B; ++i)
        aux[i] = total ^ bids[i];
    std::sort(aux.begin(), aux.end());

    std::vector<std::vector<uint64_t>> pref_x(S + 1, {0});
    std::vector<std::vector<uint64_t>> pref_y(S + 1, {0});
    for (int m = 1; m <= S; ++m)
        for (int s = m - 1; s < S; s += m) {
            const auto nx = pref_x[m].back() ^ bids[slides[s].first];
            pref_x[m].push_back(nx);
            const auto ny = pref_y[m].back() ^ bids[slides[s].second];
            pref_y[m].push_back(ny);
        }
    // applay operations
    int enabled{0};
    uint64_t hx{0}, hy{0};
    for (int z = N; z; --z) {
        char op; int l, r, m;
        std::cin >> op >> l >> r >> m;
        const int from = (l - 1) / m;
        const int to = r / m;
        const int delta = to - from;
        enabled += (op == 'E' ? delta : -delta);

        hx ^= pref_x[m][to] ^ pref_x[m][from];
        hy ^= pref_y[m][to] ^ pref_y[m][from];
        if (enabled == B - 1) {
            const auto itx = std::lower_bound(aux.begin(), aux.end(), hx);
            const auto ity = std::lower_bound(aux.begin(), aux.end(), hy);
            const auto res = slide_inverse_ids.find({hx, hy});
            if (itx != aux.end() && ity != aux.end() && res != slide_inverse_ids.end()) {
                std::cout << ' ' << res->second + 1;
                continue;
            }
        }

        std::cout << " X";
    }
}

std::vector<std::vector<std::pair<int, bool>>> graph; // {edge_to, enabled}

static void check_set1() {
    bool is_bad{};
    int candidate_from{-1}, candidate_to{-1};
    std::vector<int> in_degrees(B);
    for (int i = 0; i < B; ++i) {
        const auto& bf = graph[i];
        int out_degree{0};
        for (const auto& bt : bf)
            if (bt.second) {
                ++out_degree;
                ++in_degrees[bt.first];
            }
        if (out_degree > 1 || (out_degree == 0 && candidate_from >= 0)) {
            is_bad = true;
            break;
        } else if (out_degree == 0)
            candidate_from = i;
    }

    for (int i = 0; i < B; ++i) {
        if (in_degrees[i] > 1 || (in_degrees[i] == 0 && candidate_to >= 0)) {
            is_bad = true;
            break;
        } if (in_degrees[i] == 0)
            candidate_to = i;
    }

    if (!is_bad && candidate_from >= 0 && candidate_to >= 0) {
        for (int i = 0; i < S; ++i) {
            const auto& s = slides[i];
            if (s.first == candidate_from && s.second == candidate_to) {
                const auto& edges = graph[candidate_from];
                if (std::count(edges.begin(), edges.end(), std::pair{candidate_to, false}) == 1) {
                    std::cout << ' ' << i + 1;
                    return;
                }
            }
        }
    }

    std::cout << " X";
}

static void solve_set1() {
    std::cin >> B >> S >> N;
    // build initial state
    graph.assign(B, {});
    slides.resize(S);
    for (int i = 0; i < S; ++i) {
        int from, to; std::cin >> from >> to;
        --from, --to;
        slides[i] = {from, to};
        graph[from].emplace_back(to, false);
    }
    // applay operations
    for (int z = N; z; --z) {
        char op; int l, r, m;
        std::cin >> op >> l >> r >> m;
        for (int s = l; s <= r; ++s)
            if ((s % m) == 0) {
                const auto& slide = slides[s - 1];
                for (auto& e : graph[slide.first])
                    if (e.first == slide.second) {
                        e.second ^= true;
                        break;
                    }
            }
        check_set1();
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
        std::cout << "Case #" << g << ":"; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 slide_circuits.cpp -o slide_circuits.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address slide_circuits.cpp -o slide_circuits

Run:
slide_circuits.exe < slide_circuits.in

Input:

2
3 3 5
1 2
2 3
3 1
E 1 2 1
E 3 3 1
D 1 3 2
D 1 3 3
E 1 2 2
5 8 10
1 5
5 3
4 1
3 2
2 4
2 5
2 1
1 4
E 1 8 2
D 4 8 2
E 3 5 1
E 1 1 3
E 1 1 1
E 5 8 2
D 1 8 3
D 5 8 4
D 4 5 1
E 3 4 1

Output:

Case #1: 3 X 2 X 3
Case #2: 3 X 1 1 X X X 3 X 5

*/
