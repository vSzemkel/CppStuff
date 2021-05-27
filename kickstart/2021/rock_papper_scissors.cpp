
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>

// Rock, paper, scissors
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435c44/00000000007ec28e

constexpr int D = 60;
constexpr double edge = 1. / 3;
std::pair<double, char> dp[D + 1][D + 1][D + 1]; // >1MB, to large for stack allocation

static void solve() {
    int W, E; std::cin >> W >> E;
    /* not needed as of default constructor of std::pair
    for (int i = 0; i <= D; ++i)
        for (int j = 0; j + i <= D; ++j)
            dp[i][j][D-i-j] = {0., '-'}; */

    // P > R > S > P
    // dp[i][j][k] is maximal expected value after D - (i+j+k) moves
    double pP, pR, pS; // openent choice probabilities
    for (int p = D - 1; p >= 0; --p)
        for (int r = D - p - 1; r >= 0; --r)
            for (int s = D - p - r - 1; s >= 0; --s) {
                auto& dp_case = dp[p][r][s];
                const double after = p + r + s;
                if (after > 0) {
                    pP = r / after; 
                    pR = s / after;
                    pS = p / after;
                } else
                    pP = pR = pS = edge;

                dp_case = {-1., '-'};
                // what if I choose P
                const auto eP = dp[p + 1][r][s].first + pP * E + pR * W;
                dp_case = std::max(dp_case, {eP, 'P'});
                // what if R
                const auto eR = dp[p][r + 1][s].first + pR * E + pS * W;
                dp_case = std::max(dp_case, {eR, 'R'});
                // what if S
                const auto eS = dp[p][r][s + 1].first + pS * E + pP * W;
                dp_case = std::max(dp_case, {eS, 'S'});
            }

    int p{0}, r{0}, s{0}, m{D};
    while (m > 0) {
        const char choice = dp[p][r][s].second;
        std::cout << choice;
        if (choice == 'P') ++p;
        else if (choice == 'R') ++r;
        else ++s;
        --m;
    }
}

static void solve_set1() {
    int W, E; std::cin >> W >> E;

    const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};
    std::string ret;
    ret.resize(D);

    double p{0}, r{0}, s{0};
    ret[0] = 'P' + rand_in_range(3);
    if (ret[0] == 'Q') { ret[0] = 'S'; s += 1.0; }
    else if (ret[0] == 'P') p += 1.0;
    else r += 1.0;

    for (int d = 1; d < D; ++d) {
        std::cin >> W >> E;
        const auto pp = p / d;
        const auto ps = s / d;
        const auto pr = r / d;
        if ((pp > ps && pp > pr) || ((E <= W / 2) && pp >= ps && pp >= pr)) {
            r += 1.0;
            ret[d] = 'R';
        } else if ((ps > pp && ps > pr) || ((E <= W / 2) && ps >= pp && ps >= pr)) {
            p += 1.0;
            ret[d] = 'P';
        } else if ((pr > ps && pr > pp) || ((E <= W / 2) && pr >= ps && pr >= pp)) {
            s += 1.0;
            ret[d] = 'S';
        } else {
            ret[d] = 'P' + rand_in_range(3);
            if (ret[d] == 'Q') {
                ret[d] = 'S';
                s += 1.0;
            } else if (ret[d] == 'P') p += 1.0;
            else r += 1.0;
        }
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases, X;
    std::cin >> no_of_cases >> X;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 rock_papper_scissors.cpp -o rock_papper_scissors.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address rock_papper_scissors.cpp -o rock_papper_scissors

Run:
rock_papper_scissors.exe < rock_papper_scissors.in

Input:

2
30
60 0
60 60

Output:

Case #1: SRRRRRRRRRPPPPPPPPPPPPPPPPPPPPPPPPPPPSSSSSSSSSSSSSSSSSSSSSSS
Case #2: SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS

*/
