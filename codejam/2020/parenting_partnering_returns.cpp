
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Parenting Partnering Returns
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd27/000000000020bdf9


void solve() {
    int N;
    std::vector<std::tuple<int, int, int>> events;
    std::cin >> N;
    events.resize(N);
    for (int i = 0; i < N; ++i) {
        int begin, end;
        std::cin >> begin >> end;
        events[i] = {begin, end, i};
    }

    std::sort(events.begin(), events.end());

    std::string ret(N, ' ');
    int lastJ{0}, lastC{0};
    for (int i = 0; i < N; ++i) {
        const auto& ev = events[i];
        if (lastJ <= std::get<0>(ev)) {
            lastJ = std::get<1>(ev);
            ret[std::get<2>(ev)] = 'J';
        } else if (lastC <= std::get<0>(ev)) {
            lastC = std::get<1>(ev);
            ret[std::get<2>(ev)] = 'C';
        } else {
            std::cout << "IMPOSSIBLE";
            return;
        }
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 parenting_partnering_returns.cpp -o parenting_partnering_returns.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 parenting_partnering_returns.cpp -o parenting_partnering_returns.o
parenting_partnering_returns.exe < parenting_partnering_returns.in

Input:

4
3
360 480
420 540
600 660
3
0 1440
1 3
2 4
5
99 150
1 100
100 301
2 5
150 250
2
0 720
720 1440

Output:

Case #1: CJC
Case #2: IMPOSSIBLE
Case #3: JCCJJ
Case #4: CC

*/
