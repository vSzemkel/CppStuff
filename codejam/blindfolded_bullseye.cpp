
#include <array>
#include <iostream>
#include <fstream>
#include <utility>

// Blindfolded Bullseye
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fef2/00000000002d5b63

constexpr int g_max = 1'000'000'000;

std::ofstream g_debug("blindfolded_bullseye.log");
int g_A, g_B, g_question_asked;

enum class answer_t : uint8_t
{
    hit,
    miss,
    center,
    wrong
};

answer_t ask(const int x, const int y)
{
    std::cout << x << " " << y << std::endl;
    std::string ans;
    std::cin >> ans;
    ++g_question_asked;
    g_debug << g_question_asked << ": [" <<x << " " << y << "] -> " << ans << std::endl;
    switch (ans[0]) {
        case 'H': return answer_t::hit;
        case 'M': return answer_t::miss;
        case 'C': return answer_t::center;
        case 'W': return answer_t::wrong;
    }

    return answer_t::wrong;
}

void solve() {
    g_question_asked = 0;
    // find hit
    int x0{0}, y0{0};
    std::array<std::pair<int, int>, 8> test = {std::make_pair(-5, 0), {-5, -5}, {0, -5}, {5, -5}, {5, 0}, {5, 5}, {0, 5}, {-5, 5}};
    for (const auto& [x, y] : test)
        if (ask(g_max / 10 * x, g_max / 10 * y) == answer_t::hit) {
            x0 = g_max / 10 * x;
            y0 = g_max / 10 * y;
            break;
        }
    g_debug << "Found point within the dartboard\n";
    // find left
    int l{-g_max}, r{x0};
    while (l + 1 < r) {
        const int mid = (l + r) / 2;
        const auto midtest = ask(mid, y0);
        if (midtest == answer_t::center) return;
        if (midtest == answer_t::hit) 
            r = mid;
        else
            l = mid;
    }
    int left = r;
    g_debug << "Found left point within the dartboard: " << left << "\n";
    // find right
    l = x0; r = g_max;
    while (l + 1 < r) {
        const int mid = (l + r) / 2;
        const auto midtest = ask(mid, y0);
        if (midtest == answer_t::center) return;
        if (midtest == answer_t::hit) 
            l = mid;
        else
            r = mid;
    }
    int right = l;
    g_debug << "Found right point within the dartboard: " << right << "\n";
    // find top
    int t{g_max}, b{y0};
    while (b + 1 < t) {
        const int mid = (t + b) / 2;
        const auto midtest = ask(x0, mid);
        if (midtest == answer_t::center) return;
        if (midtest == answer_t::hit) 
            b = mid;
        else
            t = mid;
    }
    int top = b;
    g_debug << "Found top point within the dartboard: " << top << "\n";
    // find bottom
    t = y0; b = -g_max;
    while (b + 1 < t) {
        const int mid = (t + b) / 2;
        const auto midtest = ask(x0, mid);
        if (midtest == answer_t::center) return;
        if (midtest == answer_t::hit) 
            t = mid;
        else
            b = mid;
    }
    int bottom = t;
    g_debug << "Found bottom point within the dartboard: " << bottom << "\n";
    ask((left + right) / 2, (top + bottom) / 2);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases >> g_A >> g_B;
    g_debug << "Parameters: " << no_of_cases << " " << g_A << " " << g_B << "\n";
    for (int g = 1; g <= no_of_cases; ++g) {
        solve();
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 blindfolded_bullseye.cpp -o blindfolded_bullseye.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 blindfolded_bullseye.cpp -o blindfolded_bullseye.o
cls & py.exe interactive_runner.py py.exe blindfolded_bullseye_testing_tool.py 1 -- blindfolded_bullseye.exe

Input:


Output:


*/
