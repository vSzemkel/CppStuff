
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

// Duplicates in range
// Given the range in an array find duplicate element within it

template <typename C>
static void print(const C& v)
{
    if (v.empty()) return;
    std::cout << v.front();
    const int size = int(v.size());
    for (int i = 1; i < size; ++i)
        std::cout << ' ' << v[i];
    std::cout << '\n';
}

std::vector<int> input; // input data
std::vector<int> left;  // left[X] = S; right-most pair of duplicates not exceeding X starts in S
std::unordered_map<int, int> right; // right[S] = F; pair starting in S ends in F
std::unordered_map<int, int> cache; // cache[X] = K; right-most occurence of X was on position K or -1

static void check(const int i, const int j) { // [i..j]
    if (left[j] < i)
        std::cout << "No duplicates in [" << i << ", " << j << "]\n";
    else {
        const int d1 = left[j];
        const int d2 = right[d1];
        assert(input[d1] == input[d2]);
        std::cout << "Found duplicated " << input[d1] << " in [" << i << ", " << j << "] on (" << d1 << ", " << d2 << ")\n";
    }
}

int main(int, char**)
{
    input = {0, 1, 4, 3, 4, 5, 6, 7, 8, 3};
    assert(!input.empty());
    int N = int(input.size());

    // prepare
    right[-1] = 0;
    left.resize(N);
    left[0] = -1;
    cache[input.front()] = 0;
    for (int i = 1; i < N; ++i) {
        const int val = input[i];
        const int prev = (cache.count(val) > 0) ? cache[val] : -1;
        if (left[i - 1] < prev) {
            left[i] = prev;
            right[prev] = i;
        } else
            left[i] = left[i - 1];
        cache[val] = i;
    }

    std::cout << "input: "; print(input);
    std::cout << "left: "; print(left);

    check(0, 3);
    check(1, 3);
    check(1, 9);
    check(2, 4);
    check(1, 5);
    check(3, 9);
    check(5, 9);
    check(0, 9);
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 duplicates_in_range.cpp -o duplicates_in_range.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address duplicates_in_range.cpp -o duplicates_in_range

Output:

input: 0 1 4 3 4 5 6 7 8 3
left: -1 -1 -1 -1 2 2 2 2 2 3
No duplicates in [0, 3]
No duplicates in [1, 3]
Found duplicated 3 in [1, 9] on (3, 9)
Found duplicated 4 in [2, 4] on (2, 4)
Found duplicated 4 in [1, 5] on (2, 4)
Found duplicated 3 in [3, 9] on (3, 9)
No duplicates in [5, 9]
Found duplicated 3 in [0, 9] on (3, 9)

*/
