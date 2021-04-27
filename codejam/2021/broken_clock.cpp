
#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
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
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Broken clock
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435baf/00000000007ae694

constexpr int minutesPerHour = 60;
constexpr int secondsPerMinute = 60;
constexpr int64_t nanosecondsPerSecond = 1'000'000'000;
constexpr int64_t nanosecondsPerMinute = nanosecondsPerSecond * 60;
constexpr int64_t nanosecondsPerHour = nanosecondsPerMinute * 60;
constexpr int64_t angleHourPerNanosecond = 1;
constexpr int64_t angleMinutePerNanosecond = angleHourPerNanosecond * 60 / 5;
constexpr int64_t angleSecondPerNanosecond = angleMinutePerNanosecond * 60 / 1;
constexpr int64_t angleFullHour = angleHourPerNanosecond * nanosecondsPerHour;
constexpr int64_t angleFullMinute = angleMinutePerNanosecond * nanosecondsPerMinute;
constexpr int64_t angleFullSecond = angleSecondPerNanosecond * nanosecondsPerSecond;

static void solve() {
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
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
        //std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(15) << solve() << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 broken_clock.cpp -o broken_clock.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address broken_clock.cpp -o broken_clock.o

Run:
broken_clock.exe < broken_clock.in

Input:

3
0 0 0
0 21600000000000 23400000000000
1476000000000 2160000000000 3723000000000

Output:

Case #1: 0 0 0 0
Case #2: 6 30 0 0
Case #3: 1 2 3 0

*/
