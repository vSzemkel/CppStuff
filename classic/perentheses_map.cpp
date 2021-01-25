
#include <assert.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Perentheses map
// Prepare a datastructure that tells the position of matching parenthese

const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};

std::vector<int> solve_space(const std::string& ps)
{
    const auto size = (int)ps.size();
    std::vector<int> ret(size);
    int l{0}, r{size - 1}, staged{0};
    while (l < r - 1) {
        assert(ps[l] == '(');
        int m = l + 1;
        int level = 1;
        while (m <= r && level > 0) {
            if (ps[m] == '(') ++level;
            else --level;
            ++m;
        }
        assert(level == 0);
        --m;
        ret[l] = m - l;
        ret[m] = l - m;
        if (l == m - 1)
            l += 2;
        else {
            ++staged;
            ++l;
        }
        if (m == r) --r;
        while (staged > 0 && ps[l] == ')' && l <= r) {
            --staged;
            ++l;
        }
    }
    ret[l] = r - l;
    ret[r] = l - r;
    return ret;
}

std::vector<int> solve_time(const std::string& ps)
{
    const auto size = (int)ps.size();
    std::vector<int> stage, ret(size);
    stage.reserve(size / 2);
    for (int i = 0; i < size; ++i) {
        if (ps[i] == '(')
            stage.push_back(i);
        else {
            const int opening = stage.back();
            ret[opening] = i - opening;
            ret[i] = opening - i;
            stage.pop_back();
        }
    }
    return ret;
}

std::string generate(const int n)
{
    std::string ret;
    for (int i = 0; i < n; ++i)
        ret.insert(rand_in_range(ret.size() + 1), "()");
    return ret;
}

void print(const std::vector<int>& v) {
    for (const int i : v)
        std::cout << i << " ";
    std::cout << "\n";
}

void fuzz()
{
    constexpr int length = 50;
    const std::string ps = generate(length);
    const std::vector<int> space = solve_space(ps);
    const std::vector<int> time = solve_time(ps);

    std::cout << ps << "\n";
    print(space);
    std::cout << "\n";

    assert(space == time);
}

int main(int, char**)
{
    int no_of_cases = 100;
    for (int g = 1; g <= no_of_cases; ++g)
        fuzz();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 perentheses_map.cpp -o perentheses_map.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 perentheses_map.cpp -o perentheses_map.o

*/
