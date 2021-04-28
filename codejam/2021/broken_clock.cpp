
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

static bool check(const int64_t h, const int64_t m, const int64_t s)
{
    const auto HH = h / angleFullHour;
    const auto MM = m / angleFullMinute;
    const auto SS = s / angleFullSecond;
    const auto FF = s % angleFullSecond;

    const auto checkedM = MM == minutesPerHour * (h % angleFullHour) / angleFullHour;
    const auto checkedS = SS == secondsPerMinute * (m % angleFullMinute) / angleFullMinute;
    const auto checkedN = 0 == nanosecondsPerSecond * (s % angleFullSecond) / angleFullSecond;

    if (HH < 12 && MM < 60 && SS < 60 && checkedM && checkedS && checkedN) {
        const auto NN = 0;//nanosecondsPerSecond * (T[2] % angleFullSecond) * angleFullSecond;
        std::cout << HH << ' ' << MM << ' ' << SS << ' ' << NN;
        return true;
    }

    return false;
}

static void solve() {
    std::array<int64_t, 3> T;
    std::cin >> T[0] >> T[1] >> T[2];

    std::sort(T.begin(), T.end());
    T[2] -= T[0];
    T[1] -= T[0];
    T[0] = 0;

    for (int r = 0; r < secondsPerMinute; ++r) {
        do { // assume T is HH:MM:SS
            const auto secondsAdjustment = (T[2] / angleFullSecond * angleFullSecond) - T[2];
            const auto T0 = T[0] + secondsAdjustment;
            const auto T1 = T[1] + secondsAdjustment;
            const auto T2 = T[2] + secondsAdjustment;

            if (check(T0, T1, T2))
                return;
        } while (std::next_permutation(T.begin(), T.end()));

        for (auto& a : T)
            a = (a + angleFullSecond) % angle360;
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
