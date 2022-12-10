
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Latest Guests
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000051061/0000000000161427

static void solve() {
    int N, G, M;
    std::cin >> N >> G >> M;
    std::vector<std::pair<int, bool>> guests(G); // {position, clockwise}
    for (auto& g : guests) {
        char dir;
        std::cin >> g.first >> dir;
        --g.first;
        g.second = dir == 'C';
    }
    std::sort(guests.begin(), guests.end());

    int last_c{-1}, last_a{-1};
    std::vector<int> peer(G, -1), cpeer(G, -1);

    for (int i = 0; i < N; ++i)
        if (guests[i].second) {
            peer[i] = last_c;
            cpeer[i] = last_a;
            last_c = i;
        } else {
            last_a = i;
        }
    if (~last_c) {
        int i{0};
        while (true) {
            if (guests[i].second) {
                peer[i] = last_c;
                break;
            }
            cpeer[i] = last_c;
            ++i;
        }
    }

    for (int i = N - 1; ~i; --i)
        if (!guests[i].second) {
            peer[i] = last_a;
            last_a = i;
        } else
            cpeer[i] = last_a;
    if (~last_a) {
        int i{N - 1};
        while (true) {
            if (!guests[i].second) {
                peer[i] = last_a;
                break;
            }
            cpeer[i] = last_a;
            --i;
        }
    }

    for (int i = 0; i < G; ++i) {
        int peer_distance{0};
        int peer_trace = std::min(M, N);
        if (guests[i].second)
            peer_distance = (N + i - peer[i]) % N;
        else
            peer_distance = (N + peer[i] - i) % N;
        if (peer_distance <= M)
            peer_trace = peer_distance;

        int final_pos = (guests[i].first + M) % N;
        int last_collision = peer[cpeer[final_pos]];
        int colide_distance{0};
        if (guests[i].second)
            colide_distance = (N + i - last_collision) % N;
        else
            colide_distance = (N + last_collision - i) % N;

        std::cout << peer_trace - colide_distance << ' ';
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
        // /*attach_trap();*/ solve();
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
        //std::cout << "Case #" << g << ": " << (solve() ? "" : "IM") << "POSSIBLE\n";
        //std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 latest_guests.cpp -o latest_guests.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address latest_guests.cpp -o latest_guests

Run:
py.exe interactive_runner.py py.exe latest_guests_testing_tool.py 1 -- latest_guests.exe
latest_guests.exe < latest_guests.in

Input:


Output:


*/
