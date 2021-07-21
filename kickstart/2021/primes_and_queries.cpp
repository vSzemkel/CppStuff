
#include <algorithm>
#include <iostream>
#include <vector>

// Primes and queries
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000004361e3/000000000082bcf4

constexpr int MAXS = 4;

template <typename T = int64_t>
struct simple_segment_tree // can be done with ecnerwala partial_sum
{
  public:
    simple_segment_tree(const std::vector<T>& input) {
        const int input_size = (int)input.size();
        while (offset < input_size) offset *= 2;
        nodes.resize(2 * offset);
        std::copy(input.begin(), input.end(), nodes.begin() + offset);
        for (int n = offset - 1; n > 0; --n)
            nodes[n] = join(nodes[2 * n], nodes[2 * n + 1]);
    }

    T query(int lower, int upper) const { // range [lower, upper)
        T ret{};
        for (lower += offset, upper += offset; lower < upper; lower >>= 1, upper >>= 1) { 
            if (lower & 1)
                ret = join(ret, nodes[lower++]); 
            if (upper & 1)
                ret = join(ret, nodes[--upper]);
        }

        return ret;
    }

    void update(int pos, const T val) {
        pos += offset;
        nodes[pos] = val;
        while (pos > 1) {
            pos /= 2;
            nodes[pos] = join(nodes[2 * pos], nodes[2 * pos + 1]);
        }
    }

  private:
    static T join(const T& lhs, const T& rhs) {
        return lhs + rhs;
    }

    std::vector<T> nodes;
    int offset{1};
};

static std::vector<int64_t> factorize(int64_t n) {
    std::vector<int64_t> factorization;
    if (n < 1) return factorization;

    for (int d : {2, 3, 5}) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
    }

    int i{0}, increments[] = {6, 4, 2, 4, 2, 4, 6, 2};
    for (int64_t d = 7; d * d <= n; d += increments[i]) {
        while (n % d == 0) {
            factorization.push_back(d);
            n /= d;
        }
        i = (i + 1) % 8;
    }

    if (n > 1)
        factorization.push_back(n);

    return factorization;
}

static int64_t compute_segment(const int s, const int p, const int64_t n) {
    int64_t m = n % p;
    int64_t a{n}, b{m};
    for (int j = 1; j < s; ++j) a *= n; // n == A[i]**s
    for (int j = 1; j < s; ++j) b *= m; // m == (A[i] % P)**s
    auto fact = factorize(a - b);
    return std::count(fact.begin(), fact.end(), p);
}

static void solve_set1() {
    int N, Q, P; std::cin >> N >> Q >> P;
    std::vector<int64_t> A(N);
    for (auto& a : A)
        std::cin >> a;

    std::vector<int64_t> tmp(N);
    std::vector<simple_segment_tree<>> trees;
    trees.reserve(MAXS);
    for (int s = 1; s <= MAXS; ++s) {
        for (int i = 0; i < N; ++i)
            tmp[i] = compute_segment(s, P, A[i]);

        trees.push_back(simple_segment_tree{tmp});
    }

    for (int q = 0; q < Q; ++q) {
        int t; std::cin >> t;
        if (t == 1) {
            int64_t pos, val; std::cin >> pos >> val;
            --pos;
            for (int s = 1; s <= MAXS; ++s)
                trees[s - 1].update(pos, compute_segment(s, P, val));
        } else { // t == 2
            int S, L, R; 
            std::cin >> S >> L >> R;
            std::cout << ' ' << trees[S - 1].query(L - 1, R);
        }
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
        std::cout << "Case #" << g << ":"; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 primes_and_queries.cpp -o primes_and_queries.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address primes_and_queries.cpp -o primes_and_queries

Run:
primes_and_queries.exe < primes_and_queries.in

Input:

2
5 5 2
16 94 62 67 91
2 3 3 4
1 1 69
2 3 1 4
2 1 1 1
2 3 2 2
5 5 5
1 2 3 4 5
2 1 1 5
1 3 98
2 3 2 4
1 5 3
2 2 1 5

Output:

Case #1: 4 9 2 3
Case #2: 1 1 1

*/
