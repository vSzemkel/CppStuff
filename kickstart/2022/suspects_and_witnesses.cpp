
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

// Suspects and Witnesses
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caea6/0000000000b76db9

/**
 * @brief Observation: If more then K people claims that someone is innocent he truly is
 */
static void solve() { // by tabr
    int N, M; size_t K;
    std::cin >> N >> M >> K;

    std::vector<std::vector<int>> testimonials(N); // t[p] = x if every person in x claims p innocent
    for (int z = M; z; --z) {
        int a, b;
        std::cin >> a >> b;
        testimonials[--b].push_back(--a);
    }

    std::vector<int> votes(N, 1);
    for (int i = 0; i < N; ++i) {
        std::vector<int> q({i}); // stack q :)
        std::set<int> s({i});
        while (!q.empty() && s.size() <= K) {
            const int a = q.back(); q.pop_back();
            for (const int w : testimonials[a])
                if (s.insert(w).second && s.size() <= K)
                    q.push_back(w);
        }

        if (s.size() <= K) // all guilty
            for (const int g : s)
                votes[g] = 0;
    }

    std::cout << std::accumulate(votes.begin(), votes.end(), 0);
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 suspects_and_witnesses.cpp -o suspects_and_witnesses.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address suspects_and_witnesses.cpp -o suspects_and_witnesses

Run:
suspects_and_witnesses.exe < suspects_and_witnesses.in

Input:

2
3 2 2
1 2
2 3
3 3 2
1 2
2 3
3 2

Output:

Case #1: 1
Case #2: 2

*/
