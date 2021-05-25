
#include <assert.h>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <utility>

// Binary operator (inspired by Errichto)
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435c44/00000000007ec290

const auto big64 = std::numeric_limits<int64_t>::max();
const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

constexpr int M = 1'000'050'131;

std::string g_expr;
std::map<std::pair<int, int>, int64_t> g_cache;

static int balance(const char c) {
    if (c == '(') return 1;
    if (c == ')') return -1;
    return 0;
}

static int compute(const int a, const char op, const int b) {
    if (op == '+') return (int64_t(a) + b) % M;
    if (op == '*') return (int64_t(a) * b) % M;
    assert(op == '#');
    const auto found = g_cache.find({a, b});
    if (found != g_cache.end())
        return found->second;

    const int ret = rand_in_range(big64) % M;
    g_cache[{a, b}] = ret;
    return ret;
}

static int eval(const int L, const int R) { // [L, R)
    if (g_expr[L] == '(') {
        int bal{1}, pos{L + 1};
        while (bal > 0) 
            bal += balance(g_expr[pos++]);

        const int a = eval(L + 1, pos - 1);
        if (pos >= R) return a;
        const auto op = g_expr[pos];
        assert(op == '+' || op == '*' || op == '#');
        const int b = eval(pos + 1, R);
        return compute(a, op, b);
    }

    int a{0}, pos{L};
    while (pos < R && isdigit(g_expr[pos]))
        a = (a * 10LL + g_expr[pos++] - '0') % M;
    if (pos >= R) return a;

    const auto op = g_expr[pos];
    assert(op == '+' || op == '*' || op == '#');
    const int b = eval(pos + 1, R);
    return compute(a, op, b);
}

static void solve() {
    int N; std::cin >> N;

    std::map<int, int> groups;
    for (int i = 0; i < N; ++i) {
        std::cin >> g_expr;
        const int val = eval(0, g_expr.size());
        if (groups.find(val) == groups.end())
            groups[val] = groups.size();
        std::cout << groups[val] + 1 << ' ';
    }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 binary_operator.cpp -o binary_operator.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address binary_operator.cpp -o binary_operator

Run:
binary_operator.exe < binary_operator.in

Input:

3
7
(1*(1#2))
(0*(1#2))
(1#2)
0
(3*0)
((1#2)*1)
(((1+(1#2))+3)*0)
5
(1*((1+(2#2))+3))
((0+(2#2))+4)
(100#2)
(((1+(2#2))+3)*1)
((50*2)#2)
2
(9999999999999999999999999999999999999999+1)
(100000000000000000000*100000000000000000000)

Output:

Case #1: 1 2 1 2 2 1 2 
Case #2: 1 1 2 1 2
Case #3: 1 1

*/
