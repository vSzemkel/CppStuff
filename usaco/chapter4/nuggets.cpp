/*
ID: marcin.8
LANG: C++
TASK: nuggets
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=1wgmYQz2BNU&S=nuggets

Given two relatively prime numbers N and M, the largest number that 
you cannot make is NM - M - N, that is, the product minus the sum

*/

#include <fstream>
#include <queue>
#include <unordered_set>
#include <vector>

std::ifstream task_in("nuggets.in");
std::ofstream task_out("nuggets.out");

constexpr const int LIMIT = 2000000000;

int N;
std::vector<int> boxes;

static int64_t gcd(int64_t a, int64_t b)
{
    while (true) {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

static int64_t lcm(int64_t a, int64_t b)
{
    const auto temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

static int64_t decompose_naive(int64_t c) { // MLE
    std::vector<bool> state(c);
    state[0] = true;
    for (int i = 0; i < c; i++) {
        if (!state[i])
            continue;
        for (int j = 0; j < N; ++j) {
            const int sum = i + boxes[j];
            if (sum < c)
                state[sum] = true;
        }
    }

    while (state[--c])
        ;

    return c;
}

/**
 * @brief Counting from minbox we remember last decomposable and max element in queue
 * Process until there is no gaps in queue of a length at least minbox
 */
static int64_t decompose() {
    std::priority_queue<int, std::vector<int>, std::greater<>> pq;
    std::unordered_set<int> decomposable;
    int min = 1e09, max = 0;
    for (const auto& b : boxes) {
        pq.push(b);
        decomposable.insert(b);
        max = std::max(max, b);
        min = std::min(min, b);
    }

    int prev = min - 1;
    int gap = prev; // maximal not decomposable value
    while (true) {
        const auto can = pq.top(); pq.pop();
        if (prev + 1 < can)
            gap = can - 1;
        if (max - can >= min && pq.size() == size_t(max - can))
            break;
        prev = can;
        decomposable.erase(can);
        for (const auto& b : boxes) {
            const auto val = can + b;
            const auto [_, inserted] = decomposable.insert(val);
            if (inserted) {
                pq.push(val);
                max = std::max(max, val);
            }
        }
    }

    return gap;
}

int main(int, char**)
{
    int max{0};
    task_in >> N;
    boxes.resize(N);
    for (auto& b : boxes) {
        task_in >> b;
        max = std::max(max, b);
    }

    int g = boxes.front();
    int64_t m = g;
    for (int i = 1; i < N; ++i) {
        g = gcd(g, boxes[i]);
        m = lcm(m, boxes[i]);
    }

    if (g > 1 || m == 1) // inf many || every one
        task_out << "0\n";
    else
        task_out << decompose() << '\n';
        //task_out << decompose_naive(std::min(m, 2000000000LL)) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 nuggets.cpp -o nuggets.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address nuggets.cpp -o nuggets

Input:

5 
251 
252 
250 
254 
256

Output:

10749

*/
