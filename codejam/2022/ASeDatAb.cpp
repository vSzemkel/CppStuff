
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

// ASeDatAb
// https://codingcompetitions.withgoogle.com/codejam/round/000000000087711b/0000000000acd29b

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

constexpr const int L = 8;
static const auto all0 = std::string(L, '0'), all1 = std::string(L, '1');

/**
 * @brief Constructive approach. Build a sequence that solves task for L == (1 << x)
 * 
 */
static void solve() {
    std::vector<std::string> seq(1, "1");
    for (int tour = 1; (1 << tour) <= L; ++tour) {
        auto copy_zero = seq;
        for (auto& s : copy_zero) s += all0.substr(0, s.size());
        auto copy_dub = seq;
        for (auto& s : copy_dub) s += s;
        auto next = copy_dub;
        for (const auto& s : copy_zero) {
            next.push_back(s);
            next.insert(next.end(), copy_dub.begin(), copy_dub.end());
        }
        seq = std::move(next);
    }

    for (const auto& probe : seq) {
        std::cout << probe << std::endl;
        int N; std::cin >> N;
        if (!N) break;
    }
}

static std::string rndstr() {
    std::string ret(L, '0');
    for (int p = 0; p < L; ++p)
        ret[p] = (rand_in_range(2) & 1) + '0';
    return ret;
}

static void solve_rnd() { // defeat randomness with randomness
    int N, cnt{0};

    do {
        std::cout << rndstr() << std::endl;
        std::cin >> N;
    } while (2 < N && N < L - 1);

    if (N > 4) {
        std::cout << all1 << std::endl;
        std::cin >> N;
    }

    if (N) {
        std::string s{all0};
        for (int z = N; z; --z)
            s[L - z] = '1';
        std::vector<std::string> cand;
        do {
            cand.push_back(s);
        } while (std::next_permutation(s.begin(), s.end()));

        while (true) {
            auto probe = cand[rand_in_range(cand.size())];
            std::cout << probe << std::endl;
            std::cin >> N;
            if (!N) break;

            std::unordered_set<std::string> next;
            for (int r = 0; r < L; ++r) {
                std::rotate(probe.begin(), probe.begin() + 1, probe.end());
                for (const auto& s : cand) {
                    int cnt{0};
                    auto n = s;
                    for (int i = 0; i < L; ++i)
                        if (s[i] == probe[i])
                            n[i] = '0';
                        else {
                            ++cnt;
                            n[i] = '1';
                        }
                    if (cnt == N)
                        next.insert(n);
                }
            }

            cand = std::vector<std::string>{next.begin(), next.end()};
        }
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
        solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 ASeDatAb.cpp -o ASeDatAb.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address ASeDatAb.cpp -o ASeDatAb

Run:
py.exe interactive_runner.py py.exe ASeDatAb_testing_tool.py 0 -- ASeDatAb.exe

*/
