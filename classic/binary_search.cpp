
#include <assert.h>
#include <iostream>

// Binary search
// For arbitrary n, find lowest value of increasing function that is not less then f(n)
// When range to search is too big to fit in memory, one can not use std::lower_bound

constexpr int64_t MAX = 1'000'000'000'000'000'000;

static int64_t increasing_func(const int64_t n) {
    if (MAX > 1974 && (MAX - 1974) / 33 < n)
        return MAX; // overflow
    return 33 * n + 1974;
}

static int64_t binary_search(const int64_t n) {
    int64_t lb{0}, ub{2 * MAX};
    while (lb != ub) {
        const auto mid = (lb + ub) / 2;
        if (increasing_func(mid) > n)
            ub = mid;
        else
            lb = mid + 1;
    }

    return ub;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cout << "f(0) == " << increasing_func(0) << ", f(1) == " << increasing_func(1) << '\n';
    assert(binary_search(0) == 0);
    assert(binary_search(32) == 0);
    assert(binary_search(33) == 0);
    assert(binary_search(1973) == 0);
    assert(binary_search(1974) == 1);

    std::cout << "f(32) == " << increasing_func(32) << ", f(33) == " << increasing_func(33) << '\n';
    assert(binary_search(3000) == 32);
    assert(binary_search(3029) == 32);
    assert(binary_search(3030) == 33);
    assert(binary_search(3031) == 33);
    assert(binary_search(3062) == 33);
    assert(binary_search(3063) == 34);
    assert(binary_search(3064) == 34);

    const auto BIG = (MAX - 1974) / 33; // increasing_func(BIG) not overfloving
    assert(increasing_func(MAX) == MAX); // overflow
    assert(binary_search(increasing_func(BIG) - 1) == BIG);
    assert(binary_search(increasing_func(BIG)) == BIG + 1);

    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 binary_search.cpp -o binary_search.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address binary_search.cpp -o binary_search.o

Run:
py.exe interactive_runner.py py.exe binary_search_testing_tool.py 1 -- binary_search.exe
binary_search.exe < binary_search.in

Input:


Output:


*/
