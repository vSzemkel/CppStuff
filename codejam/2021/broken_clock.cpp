
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <numeric>

// Broken clock
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435baf/00000000007ae694

constexpr int64_t minutesPerHour = 60;
constexpr int64_t secondsPerMinute = 60;
constexpr int64_t nanosecondsPerSecond = 1'000'000'000;
constexpr int64_t nanosecondsPerMinute = nanosecondsPerSecond * 60;
constexpr int64_t nanosecondsPerHour = nanosecondsPerMinute * 60;
// angleXxxxPerNanosecond is a measure of speed (distance / time)
constexpr int64_t angleHourPerNanosecond = 1;
constexpr int64_t angleMinutePerNanosecond = angleHourPerNanosecond * 60 / 5;
constexpr int64_t angleSecondPerNanosecond = angleMinutePerNanosecond * 60 / 1;
constexpr int64_t angleFullHour = angleHourPerNanosecond * nanosecondsPerHour;
constexpr int64_t angleFullMinute = angleMinutePerNanosecond * nanosecondsPerMinute;
constexpr int64_t angleFullSecond = angleSecondPerNanosecond * nanosecondsPerSecond;
constexpr int64_t angle360 = angleFullSecond * secondsPerMinute;

static void solve_set2() {
    std::array<int64_t, 3> C, T;
    std::cin >> T[0] >> T[1] >> T[2];
    std::sort(T.begin(), T.end());

    for (int h = 0; h < 12; ++h)
        for (int m = 0; m < 60; ++m)
            for (int s = 0; s < 60; ++s) {
                auto ns_count = h * nanosecondsPerHour + m * nanosecondsPerMinute + s * nanosecondsPerSecond;
                C[0] = ns_count * angleHourPerNanosecond;
                ns_count -= h * nanosecondsPerHour;
                C[1] = ns_count * angleMinutePerNanosecond;
                ns_count -= m * nanosecondsPerMinute;
                C[2] = ns_count * angleSecondPerNanosecond;
                do {
                    const auto C01 = (C[0] - C[1] + angle360) % angle360;
                    const auto T01 = (T[0] - T[1] + angle360) % angle360;
                    const auto C12 = (C[1] - C[2] + angle360) % angle360;
                    const auto T12 = (T[1] - T[2] + angle360) % angle360;
                    if (C01 == T01 && C12 == T12) {
                        std::cout << h << ' ' << m << ' ' << s << ' ' << 0;
                        return;
                    }
                } while (std::next_permutation(T.begin(), T.end()));
            }
}

static void solve_set1() {
    std::array<int64_t, 3> T;
    std::cin >> T[0] >> T[1] >> T[2];

    do { // assume T is HH:MM:SS
        const int HH = T[0] / angleFullHour;
        const int MM = T[1] / angleFullMinute;
        const int SS = T[2] / angleFullSecond;

        const auto maybeMM = minutesPerHour * (T[0] % angleFullHour) / angleFullHour;
        const auto maybeSS = secondsPerMinute * (T[1] % angleFullMinute) / angleFullMinute;

        if (HH < 12 && MM < 60 && SS < 60 && maybeMM == MM && maybeSS == SS) {
            const int NN = nanosecondsPerSecond * (T[2] % angleFullSecond) * angleFullSecond;
            std::cout << HH << ' ' << MM << ' ' << SS << ' ' << NN;
            return;
        }
    } while (std::next_permutation(T.begin(), T.end()));
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set2(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 broken_clock.cpp -o broken_clock.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address broken_clock.cpp -o broken_clock.o

Run:
broken_clock.exe < broken_clock.in

Input:

7
0 0 0
0 21600000000000 23400000000000
1476000000000 2160000000000 3723000000000
5400000000000 5400000000000 5400000000000
10800000000000 32400000000000 34200000000000
23076000000000 23760000000000 25323000000000
0 11 719

Output:

Case #1: 0 0 0 0
Case #2: 6 30 0 0
Case #3: 1 2 3 0
Case #4: 0 0 0 0
Case #5: 0 30 0 0
Case #6: 1 2 3 0
Case #7: 0 0 0 1

*/
