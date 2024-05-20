
#include <algorithm>
#include <iostream>
#include <vector>

// Median of Two Sorted Arrays
// https://leetcode.com/problems/median-of-two-sorted-arrays

double findMedianSortedArrays(const std::vector<int>& left, const std::vector<int>& right) { // O(log(M + N))
    const int lsz = int(left.size());
    const int rsz = int(right.size());
    if (lsz > rsz)
        return findMedianSortedArrays(right, left);
    if (lsz == 0)
        return rsz % 2 ? right[rsz / 2] : (right[rsz / 2] + right[rsz / 2 - 1]) / 2.0;


    const int sz = lsz + rsz;
    const int partition = (sz + 1) / 2;
    int lbeg{0}, rbeg{lsz};
    while (lbeg <= rbeg) {
        const auto lcut = (lbeg + rbeg) / 2;
        const auto rcut = partition - lcut;

        const auto l1 = lcut == 0 ? INT_MIN : left[lcut - 1];
        const auto r1 = lcut >= lsz ? INT_MAX : left[lcut];
        const auto l2 = rcut == 0 ? INT_MIN : right[rcut - 1];
        const auto r2 = rcut >= rsz ? INT_MAX : right[rcut];

        if (l1 <= r2 && l2 <= r1) {
            if (sz % 2 == 1) {
                return std::max(l1, l2);
            } else {
                return (std::max(l1, l2) + std::min(r1, r2)) / 2.0;
            }
        } else if (l1 > r2) {
            rbeg = lcut - 1;
        } else {
            lbeg = lcut + 1;
        }
    }

    return 0.0;
}

static void io_handler()
{
    int M, N;
    std::cin >> M >> N;
    std::vector<int> left(M), right(N);
    for (auto& i : left) std::cin >> i;
    for (auto& i : right) std::cin >> i;

    std::cout << findMedianSortedArrays(left, right);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": ";
        io_handler();
        std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 median_of_two_sorted.cpp -o median_of_two_sorted.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address median_of_two_sorted.cpp -o median_of_two_sorted

Run:
median_of_two_sorted.exe < median_of_two_sorted.in

Input:

1
5 7
1 2 3 4 5
6 7 8 9 10 11 12

Output:

Case #1: 6.5

*/
