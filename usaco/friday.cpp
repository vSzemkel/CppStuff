/*
ID: marcin.8
LANG: C++
TASK: friday
CONTENT: https://train.usaco.org/usacoprob2?a=F3EJ5Ggwb1V&S=ride
*/

#include <array>
#include <cassert>
#include <iostream>
#include <fstream>

std::ifstream task_in("friday.in");
std::ofstream task_out("friday.out");

constexpr int days_per_week = 7;

constexpr int is_leap(const int year) noexcept
{
    if (year % 100 == 0)
        return year % 400 == 0 ? 1 : 0; 
    else
        return year % 4 == 0 ? 1 : 0; 
};

constexpr int days_in_month(const int month, const int year) noexcept
{
    assert(1 <= month && month <= 12);
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        return 31;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 28 + is_leap(year);
}

int main(int, char**)
{
    int years; task_in >> years;
    // Monday == 1, January == 1
    int first = 1; // January, 1th of 1900 was on Monday
    std::array<int, days_per_week> days{}; // Sun, Mon, .. Sat
    for (int y = 1900; y < 1900 + years; ++y)
        for (int m = 1; m <= 12; ++m) {
            const int dim = days_in_month(m, y);
            ++days[(first + 12) % days_per_week];
            first = (first + dim) % days_per_week;
        }

    task_out << days[6] << " " << days[0] << " " << days[1] << " " << days[2] << " " << days[3] << " " << days[4] << " " << days[5] << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 friday.cpp -o friday.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 friday.cpp -o friday.o

Input:

20

Output:

36 33 34 33 35 35 34

*/
