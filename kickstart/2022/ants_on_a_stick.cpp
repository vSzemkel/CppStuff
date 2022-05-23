
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Ants on a Stick
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb4d1/0000000000b209bc


struct ant_t {
    int index; // index in orginal sequence
    int pos;   // initial position
    bool l2r;  // moving initially in letf-to-right direction
    bool operator<(const ant_t& other) const {
        return pos < other.pos;
    }
};

/**
 * @brief No matter which one ant, we know that after dist seconds 
 * some and going in l2r direction will eventually fall down
 */
struct event_t {
    int dist;
    bool l2r;
    bool operator<(const event_t& other) const {
        return dist < other.dist;
    }
};

static void solve() {
    int N, L;
    std::cin >> N >> L;
    std::vector<ant_t> stick(N); // 0: left, 1:right
    std::vector<event_t> events(N);
    for (int i = 0; i < N; ++i) {
        auto& a = stick[i];
        a.index = i + 1;
        std::cin >> a.pos >> a.l2r;
        events[i] = {a.l2r ? L - a.pos : a.pos, a.l2r};
    }

    std::sort(stick.begin(), stick.end());
    std::sort(events.begin(), events.end());

    int lcan{0}, rcan{N - 1};
    std::vector<std::pair<int, int>> ans; // time, index
    for (const auto& e : events) {
        int winner;
        if (e.l2r) // next ant will fall down from right
            winner = stick[rcan--].index;
        else
            winner = stick[lcan++].index;
       ans.emplace_back(e.dist, winner);
    }

    std::sort(ans.begin(), ans.end()); // only to break time ties in favor of the lower indexed
    for (const auto& a : ans)
        std::cout << a.second << ' ';
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
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 ants_on_a_stick.cpp -o ants_on_a_stick.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address ants_on_a_stick.cpp -o ants_on_a_stick

Run:
ants_on_a_stick.exe < ants_on_a_stick.in

Input:

4
1 5
1 1
2 7
4 1
5 0
4 10
8 0
2 1
6 1
4 0
2 10
2 0
9 0

Output:

Case #1: 1 
Case #2: 2 1
Case #3: 1 2 3 4
Case #4: 1 2

*/
