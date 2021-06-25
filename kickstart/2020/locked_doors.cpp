
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <utility>
#include <vector>

// Locked Doors
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff08/0000000000386d5c

template <typename K, typename V>
struct cache_t {
    void add(const K& key, const V& value) {
        _cache[key] = value;
    }

    std::optional<V> find(const K& key) {
        auto cached = _cache.find(key);
        if (cached == _cache.end())
            return std::nullopt;
        return cached->second;
    }
  private:
    std::map<K, V> _cache;
};

static void solve_set1_faster() {
    int N, Q; std::cin >> N >> Q;
    std::vector<int> doors(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    doors.push_back(std::numeric_limits<int>::max());

    std::vector<int> left_greater(N), right_greater(N), stack;
    for (int i = 0; i < N; ++i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            right_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        right_greater[stack.back()] = -1;
        stack.pop_back();
    }
    for (int i = N - 1; i >= 0; --i) {
        while (!stack.empty() && doors[stack.back()] < doors[i]) {
            left_greater[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    while (!stack.empty()) {
        left_greater[stack.back()] = -1;
        stack.pop_back();
    }

    cache_t<std::pair<int, int>, int> cache;
    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        const auto cached = cache.find({S, K});
        if (cached != std::nullopt) {
            std::cout << cached.value() << ' ';
            continue;
        }

        int r = S;
        int l = S - 1;
        int ans{S}, counter{K};
        while (--counter > 0)
            if (l >= 0 && doors[l] < doors[r]) {
                int bound = (r > N - 1) ? l - 1 : left_greater[r];
                if (bound == -1) bound = -1;
                if (l - bound > counter) {
                    ans = l - counter + 1;
                    break;
                }
                counter -= l - bound - 1;
                ans = bound + 1;
                l = bound;
            } else {
                int bound = (l < 0) ? r + 1 : right_greater[l];
                if (bound == -1) bound = N;
                if (bound - r > counter) {
                    ans = r + counter;
                    break;
                }
                counter -= bound - r - 1;
                r = bound;
                ans = r;
            }

        ++ans;
        std::cout << ' ' << ans;
        cache.add({S, K}, ans);
    }
}

static void solve_set1() {
    int N, Q; std::cin >> N >> Q;
    std::vector<int> doors(N - 1);
    for (auto& d : doors)
        std::cin >> d;
    doors.push_back(std::numeric_limits<int>::max());

    for (int q = 0; q < Q; ++q) {
        int S, K; std::cin >> S >> K; --S;
        int ans{S};
        int l = S - 1;
        int r = S;
        while (--K > 0) {
            if (l >= 0 && doors[l] < doors[r]) {
                ans = l;
                --l;
            } else {
                ++r;
                ans = r;
            }
        }

        std::cout << ' ' << ans + 1;
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
        std::cout << "Case #" << g << ":"; solve_set1_faster(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 locked_doors.cpp -o locked_doors.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address locked_doors.cpp -o locked_doors

Run:
locked_doors.exe < locked_doors.in

Input:

2
5 4
90 30 40 60
3 4
3 1
1 5
4 3
10 2
6 2 4 5 9 30 7 1 8
6 8
6 8

Output:

Case #1: 5 3 5 2
Case #2: 8 8

*/
