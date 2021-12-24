/*
ID: marcin.8
LANG: C++
TASK: fracdec
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=A0kIGYpcC8f&S=fracdec
*/

#include <array>
#include <assert.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr const int MAX_LINE = 76;

std::ifstream task_in("fracdec.in");
std::ofstream task_out("fracdec.out");

static std::array<std::string, 2> to_decimal_string(const int num, const int denum) {
    assert(denum > 0);
    auto pref = std::to_string(num / denum);
    const int rem = num % denum;
    if (rem == 0)
        return { pref + ".0", {} };

    pref.push_back('.');
    int counter{0};
    int val = rem * 10;
    std::unordered_map<int, int> state;
    while (true) {
        const auto found = state.find(val);
        if (found != state.end()) {
            const int pref_len = pref.size() - counter + found->second;
            const auto period = pref.substr(pref_len);
            pref.resize(pref_len);
            return { pref, period };
        }

        const int next_digit = val / denum;
        pref.push_back('0' + next_digit);
        if (val % denum == 0)
            return { pref, {} };
        state[val] = counter++;
        val = (val - next_digit * denum) * 10;
    }
}

int main(int, char**)
{
    int num, denum;
    task_in >> num >> denum;
    auto decim = to_decimal_string(num, denum);
    if (!decim[1].empty())
        decim[0] += '(' + decim[1] + ')';
    int written{0};
    for (const char c : decim[0]) {
        task_out << c;
        ++written;
        if ((written % MAX_LINE) == 0)
            task_out << '\n';
    }
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 fracdec.cpp -o fracdec.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address fracdec.cpp -o fracdec

Input:


Output:


*/
