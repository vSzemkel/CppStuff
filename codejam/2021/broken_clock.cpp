
#include <algorithm>
#include <array>
#include <assert.h>
#include <iostream>
#include <optional>
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

template <typename INT_T = int, INT_T M = 998244353>
class modnum_t {
  public:
    modnum_t() : value(0) {}
    modnum_t(int64_t v) : value(v % M) {
        if (value < 0) value += M;
    }

    modnum_t inv() const
    {
        modnum_t res;
        res.value = minv(value, M);
        return res;
    }

    modnum_t neg() const
    {
        modnum_t res;
        res.value = value ? M - value : 0;
        return res;
    }

    modnum_t operator-() const
    {
        return neg();
    }

    modnum_t operator+() const
    {
        return modnum_t(*this);
    }

    modnum_t& operator+=(const modnum_t& o)
    {
        value += o.value;
        if (value >= M) value -= M;
        return *this;
    }

    modnum_t& operator-=(const modnum_t& o)
    {
        value -= o.value;
        if (value < 0) value += M;
        return *this;
    }

    modnum_t& operator*=(const modnum_t& o)
    {
        value = INT_T(int64_t(value) * int64_t(o.value) % M);
        return *this;
    }

    modnum_t& operator/=(const modnum_t& o)
    {
        return *this *= o.inv();
    }

    modnum_t& operator++()
    {
        value++;
        if (value == M) value = 0;
        return *this;
    }

    modnum_t& operator--()
    {
        if (value == 0) value = M;
        value--;
        return *this;
    }

    friend modnum_t operator++(modnum_t& a, int)
    {
        modnum_t r = a;
        ++a;
        return r;
    }

    friend modnum_t operator--(modnum_t& a, int)
    {
        modnum_t r = a;
        --a;
        return r;
    }

    friend std::optional<modnum_t> inv(const modnum_t& m) { const auto ret = m.inv(); return ret.value < 0 ? std::nullopt : std::optional{ret}; }
    friend modnum_t neg(const modnum_t& m) { return m.neg(); }

    friend std::ostream& operator<<(std::ostream& out, const modnum_t& n) { return out << INT_T(n); }
    friend std::istream& operator>>(std::istream& in, modnum_t& n) { int64_t v; in >> v; n = modnum_t(v); return in; }

    friend bool operator<(const modnum_t& a, const modnum_t& b) { return a.value < b.value; }
    friend bool operator==(const modnum_t& a, const modnum_t& b) { return a.value == b.value; }
    friend bool operator!=(const modnum_t& a, const modnum_t& b) { return a.value != b.value; }
    friend modnum_t operator+(const modnum_t& a, const modnum_t& b) { return modnum_t(a) += b; }
    friend modnum_t operator-(const modnum_t& a, const modnum_t& b) { return modnum_t(a) -= b; }
    friend modnum_t operator*(const modnum_t& a, const modnum_t& b) { return modnum_t(a) *= b; }
    friend modnum_t operator/(const modnum_t& a, const modnum_t& b) { return modnum_t(a) /= b; }
    friend INT_T operator%(const modnum_t& a, const INT_T b) { return a.value % b; }

    explicit operator int() const { return value; }
    explicit operator int64_t() const { return value; }

  private:
    static INT_T minv(INT_T a, const INT_T m) {
        a %= m;
        if (a == 0) return -1; // not exists
        return a == 1 ? 1 : INT_T(m - int64_t(minv(m, a)) * int64_t(m) / a);
    }

    INT_T value;
};

using mod_t = modnum_t<int64_t, angle360>;

constexpr int64_t MAX = angle360;

int64_t add(int64_t a, int64_t b)
{
    a += b;
    while (a >= MAX)
        a -= MAX;
    return a;
}

int64_t modmul(int64_t a, int64_t b)
{
    int64_t r{0};
    while (b) {
        if (b & 1) r = add(r, a);
        b >>= 1;
        a = add(a, a);
    }
    return r;
}

/* Time to find is equal to HH * angleFullHour + inHourHoursShift
 * In the last partial hour, minutes hand moved (angleMinutePerNanosecond / angleHourPerNanosecond)
 * more distance then hours hand - which is equal to 12
 * Angle between hours and minutes hands is (HH * angleFullHour + inHourHoursShift) - (12 * inHourHoursShift)
 * So inHourHoursShift == (HH * angleFullHour - angleHM) / 11 and only valid candidate for HH can pass this
 */

constexpr int64_t INV11 = 15709090909091LL; // INV11 * 11 == 1 mod angle360

static void solve() {
    std::array<mod_t, 3> T;
    std::cin >> T[0] >> T[1] >> T[2];

    do {
        auto angleHM = T[1] - T[0];
        const auto inHourHoursShift = modmul(int64_t(angleHM), INV11);
        const auto fullHoursShift = mod_t{11 * inHourHoursShift} - angleHM;
        auto nanoseconds = fullHoursShift + inHourHoursShift;

        const auto start = T[0] - nanoseconds;
        if ((start + 12 * inHourHoursShift) != T[1] ) continue;
        if ((start + 720 * inHourHoursShift) != T[2] ) continue;

        const int h = int64_t(nanoseconds) / angleFullHour;
        nanoseconds -= h * angleFullHour;
        const int m = int64_t(nanoseconds) / nanosecondsPerMinute % minutesPerHour;
        nanoseconds -= m * angleFullMinute;
        const int s = int64_t(nanoseconds) / nanosecondsPerSecond % secondsPerMinute;
        nanoseconds -= s * angleFullSecond;

        std::cout << h << ' ' << m << ' ' << s << ' ' << nanoseconds % nanosecondsPerSecond;
        return;
    } while (std::next_permutation(T.begin(), T.end()));
}

static void solve_set3() {
    std::array<mod_t, 3> T;
    std::cin >> T[0] >> T[1] >> T[2];

    do {
        int n{0};
        auto angleHM = T[1] - T[0];
        while (n < 12) {
            if (angleHM % 11 == 0)
                break;
            angleHM += angleFullHour;
            ++n;
        }

        if (n == 12) continue;
        const auto inHourHoursShift = int64_t(angleHM) / 11;
        auto nanoseconds = mod_t{n * angleFullHour} + inHourHoursShift;
        const auto start = T[0] - nanoseconds;

        if ((start + 12 * inHourHoursShift) != T[1] ) continue;
        if ((start + 720 * inHourHoursShift) != T[2] ) continue;

        const int h = int64_t(nanoseconds) / angleFullHour;
        nanoseconds -= h * angleFullHour;
        const int m = int64_t(nanoseconds) / nanosecondsPerMinute % minutesPerHour;
        nanoseconds -= m * angleFullMinute;
        const int s = int64_t(nanoseconds) / nanosecondsPerSecond % secondsPerMinute;
        nanoseconds -= s * angleFullSecond;

        std::cout << h << ' ' << m << ' ' << s << ' ' << nanoseconds % nanosecondsPerSecond;
        return;
    } while (std::next_permutation(T.begin(), T.end()));
}

static void solve_set2() {
    std::array<mod_t, 3> C, T;
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
                    if (C[0] - C[1] == T[0] - T[1] && C[1] - C[2] == T[1] - T[2]) {
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
