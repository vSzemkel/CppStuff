
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Prime Time
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043585d/00000000007543d8


static void solve() {
    int64_t S{0}, M; std::cin >> M;
    std::vector<std::pair<int16_t, int64_t>> data(M);
    for (auto& d : data) {
        std::cin >> d.first >> d.second;
        S += d.first * d.second;
    }
    std::reverse(data.begin(), data.end());

    int64_t ret{S}, magic{S - 4000}; // MAGIC NUMBER FROM ESTIMATION
    while (ret > magic) {
        int64_t m{ret}, s{0};
        for (const auto& p : data) {
            for (int64_t i = 0; i < p.second; ++i)
                if (m % p.first == 0) {
                    m /= p.first;
                    s += p.first;
                } else
                    break;
            if (m % p.first == 0)
                break;
            if (S - s == ret && m == 1) {
                std::cout << (S - s);
                return;
            }
        }
        --ret;
    }

    std::cout << 0;
}

static void solve_set2() {
    int S{0}, M; std::cin >> M;
    std::vector<int> data;
    for (int i = 0; i < M; ++i) {
        int P, N; std::cin >> P >> N;
        S += P * N;
        for (int j = 0; j < N; ++j)
            data.push_back(P);
    }
    std::reverse(data.begin(), data.end());

    int64_t ret{499 * 100};
    while (ret > 0) {
        int64_t m{ret}, s{0};
        for (const auto p : data) {
            if (m % p == 0) {
                m /= p;
                s += p;
            }
            if (S - s == ret && m == 1) {
                std::cout << (S - s);
                return;
            }
        }
        --ret;
    }

    std::cout << 0;
}

static void brute() {
    int S{0}, M; std::cin >> M;
    std::vector<int> data;
    for (int i = 0; i < M; ++i) {
        int P, N; std::cin >> P >> N;
        S += P * N;
        for (int j = 0; j < N; ++j)
            data.push_back(P);
    }

    uint64_t ret{0};
    const uint64_t B = (1 << data.size()) - 1;
    for (uint64_t i = 1; i < B; ++i) {
        uint64_t x{0}, m{1}, mask{1}, j{0};
        while (mask < B) {
            if (i & mask) {
                m *= data[j];
                x += data[j];
            }
            mask *= 2;
            ++j;
        }
        const auto sum = S - x;
        if (sum == m)
            ret = std::max(ret, sum);
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
        //std::cout << "Case #" << g << ": " << std::setprecision(15) << solve() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 prime_time.cpp -o prime_time.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address prime_time.cpp -o prime_time.o

Run:
prime_time.exe < prime_time.in

Input:

4
5
2 2
3 1
5 2
7 1
11 1
1
17 2
2
2 2
3 1
1
2 7

Output:

Case #1: 25
Case #2: 17
Case #3: 0
Case #4: 8

*/
