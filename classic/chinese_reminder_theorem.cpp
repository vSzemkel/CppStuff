
#include <cassert>
#include <iostream>

// Chinese Reminder Theorem
// Given an k relative prime integers [n1..nk] and k rests [r1..rk]: 0 <= ri < ni
// find a number x: x==r1 mod n1, ..., x==rk mod nk

template <typename T>
static T minv(T a, const T m) {
    a %= m;
    if (a == 0) return -1; // not exists
    return a == 1 ? 1 : T(m - int64_t(minv(m, a)) * int64_t(m) / a);
}

int main(int, char**)
{
    const int n1{5}, n2{9}, n3{11};
    const int r1{2}, r2{7}, r3{8};

    const int n = n1 * n2 * n3;
    const int m1 = n / n1;
    const int c1 = m1 * minv(m1, n1);
    const int m2 = n / n2;
    const int c2 = m2 * minv(m2, n2);
    const int m3 = n / n3;
    const int c3 = m3 * minv(m3, n3);
    const int ans = (r1 * c1 + r2 * c2 + r3 * c3) % n;

    assert(ans == 52);
    assert(ans % n1 == r1 % n1);
    assert(ans % n2 == r2 % n2);
    assert(ans % n3 == r3 % n3);
    std::cout << "PASSED\n";
}

// clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 chinese_reminder_theorem.cpp -o chinese_reminder_theorem.exe
