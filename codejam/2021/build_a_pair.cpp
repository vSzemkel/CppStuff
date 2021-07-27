
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

// Build A Pair
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000436142/0000000000813aa8

constexpr int BASE = 10;
std::array<int, BASE> counts;
int64_t longer, shorter, ans;
int cnt;

static int64_t finish() {
    int lb{0}, rb{BASE - 1};
    while (cnt) {
        while (counts[lb] == 0) ++lb;
        longer *= BASE;
        longer += lb;
        --counts[lb];
        while (counts[rb] == 0) --rb;
        shorter *= BASE;
        shorter += rb;
        --counts[rb];
        cnt -= 2;
    }

    return longer - shorter;
}

static void solve() {
    std::string input; std::cin >> input;
    longer = shorter = ans = 0;
    cnt = int(input.size());
    counts.fill(0);
    for (const char c : input)
        ++counts[c & 0xf];

    if (cnt & 1) { // odd length - simple case
        int pos{1};
        while (counts[pos] == 0) ++pos;
        longer = pos;
        --counts[pos];
        --cnt;
        ans = finish();
    } else {
        // remember doubles 
        std::vector<int> doubles;
        for (int i = BASE - 1; i > 0; --i)
            while (counts[i] > 1) {
                doubles.push_back(i);
                counts[i] -= 2;
                cnt -= 2;
            }

        const int nonzero_size = int(doubles.size());
        if (!doubles.empty()) {
            while (counts[0] > 1) {
                doubles.push_back(0);
                counts[0] -= 2;
                cnt -= 2;
            }
        }

        if (cnt > 0) {
            const auto backup_cnt = cnt;
            const auto backup_counts = counts;
            ans = std::numeric_limits<int64_t>::max();
            const int64_t subsets = 1 << doubles.size();
            // every single double can be used in the prefix or can not (O(2**N))
            for (int64_t t = 0; t < subsets; ++t) {
                cnt = backup_cnt;
                counts = backup_counts;
                int pos{0};
                auto flag{t};
                while (flag) {
                    if (flag & 1) {
                        counts[doubles[pos]] += 2;
                        cnt += 2;
                    }
                    ++pos;
                    flag >>= 1;
                }

                const bool has_nonzero_prefix = ((1 << nonzero_size) - 1) & ~t;
                int rb = has_nonzero_prefix ? 0 : 1;

                std::map<int, std::vector<int>> distances; // {distance, starts_from}
                while (counts[rb] == 0) ++rb;
                while (true) {
                    int lb = rb++;
                    while (rb < BASE && counts[rb] == 0) ++rb;
                    if (rb == BASE) break;
                    distances[rb - lb].push_back(lb);
                }
                assert(!distances.empty());

                // here we know that tails start from different digits
                const auto& d = *distances.begin();
                const auto backup_cnt2 = cnt;
                const auto backup_counts2 = counts;
                for (const int s : d.second) {
                    cnt = backup_cnt2 - 2;
                    counts = backup_counts2;
                    longer = s + d.first;
                    shorter = s;
                    --counts[longer];
                    --counts[shorter];
                    ans = std::min(ans, finish());
                }
            }
        }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 build_a_pair.cpp -o build_a_pair.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address build_a_pair.cpp -o build_a_pair

Run:
build_a_pair.exe < build_a_pair.in

Input:

6
1234
0011
07080
0899
110108
81730972

Output:

Case #1: 7
Case #2: 0
Case #3: 620
Case #4: 1
Case #5: 2
Case #6: 3

*/
