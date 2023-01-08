
#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

// Code-Eat Switcher
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050edb/00000000001707b8

/**
 * @observation for slot with totals: TA and TB taking A we loose A * (TB / TA)
 * @conclusion take A in order starting from slots with minimal (TB / TA)
 */
static void solve() {
    int D, S;
    std::cin >> D >> S;
    std::vector<std::tuple<float, int, int>> slots(S); // {factor, code, eat}
    for (int i = 0; i < S; ++i) {
        std::cin >> std::get<1>(slots[i]) >> std::get<2>(slots[i]);
        std::get<0>(slots[i]) = float(std::get<2>(slots[i])) / std::get<1>(slots[i]); // no DBZ
    }

    std::sort(slots.begin(), slots.end());
    std::vector<int> code_psum(S + 1), eat_psum(S + 1);
    for (int i = 0; i < S; ++i) {
        code_psum[i + 1] = code_psum[i] + std::get<1>(slots[i]);
        eat_psum[i + 1] = eat_psum[i] + std::get<2>(slots[S - i - 1]);
    }

    for (int i = 0; i < D; ++i) {
        float reqc, reqe;
        std::cin >> reqc >> reqe;
        const auto fullc = std::lower_bound(code_psum.begin(), code_psum.end(), reqc) - code_psum.begin();
        const auto fulle = std::lower_bound(eat_psum.begin(), eat_psum.end(), reqe) - eat_psum.begin();
        if (fullc + fulle == S + 1) { // share slot fullc
            reqc -= code_psum[fullc - 1];
            reqe -= eat_psum[fulle - 1];
            const auto& [_, availc, availe] = slots[fullc - 1];
            std::cout << ((1 - reqc / availc) * availe >= reqe ? 'Y' : 'N');
        } else
            std::cout << (fullc + fulle < S + 1 ? 'Y' : 'N');
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 code-eat_switcher.cpp -o code-eat_switcher.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address code-eat_switcher.cpp -o code-eat_switcher

Run:
code-eat_switcher.exe < code-eat_switcher.in

Input:

2
4 2
3 8
6 10
0 18
3 13
10 0
7 3
1 2
4 4
4 4
0 0

Output:


*/
