/*
ID: marcin.8
LANG: C++
TASK: cowxor
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=VDkbdceba9s&S=cowxor
*/

#include <fstream>
#include <memory>
#include <memory_resource>
#include <unordered_map>
#include <vector>

std::ifstream task_in("cowxor.in");
std::ofstream task_out("cowxor.out");

/**
 * Fact: input[i] ^ input[i + 1] ^ .. ^ input[j] == prefix_xor[i - 1] ^ prefix_xor[j]
 * Instead of checking every pair <i, j> we use trie of all prefix_xor up to j
 * For every bit of j-th element we search for complementary bit
 */

static const int BITS = 21;
static const int high_bit = 1 << (BITS - 1);

void solution1() // MLE, TLE
{
    int N;
    task_in >> N;

    int beg{1}, end{1}, ans{};
    std::vector<int> prefix_xor(N + 1);
    std::unordered_map<int, int> trie; // {binary_prefix -> index of prefix_xor}

    for (int i = 1; i <= N; ++i) {
        int n;
        task_in >> n;
        const int cur = prefix_xor[i - 1] ^ n;
        prefix_xor[i] = cur;

        // search
        int best{0}, node{1};
        for (int bit = high_bit; bit; bit >>= 1) {
            node <<= 1;
            if (cur & bit) {
                if (trie.count(node))
                    best = trie[node];
                else
                    break; // shortcut only after bit set
            } else {
                if (trie.count(node + 1)) {
                    ++node;
                    best = trie[node];
                }
            }
        }

        // check
        const int can = prefix_xor[best] ^ cur;
        if (ans < can) {
            ans = can;
            beg = best + 1;
            end = i;
        }

        // memo
        node = 1;
        for (int bit = high_bit; bit; bit >>= 1) {
            node <<= 1;
            if (cur & bit)
                ++node;
            trie[node] = i;
        }
    }

    task_out << ans << ' ' << beg << ' ' << end << '\n';
}

struct node_t;
struct no_op {
    void operator()(node_t*) const {}
};
using node_ptr_t = std::unique_ptr<node_t, no_op>;

struct node_t
{
    node_t(int n) : _node(n) {}
    operator int() const { return _node; }
    void operator=(int n) { _node = n; }

    int _node{};
    node_ptr_t left{};
    node_ptr_t right{};
};

void solution2()
{
    int N;
    task_in >> N;

    constexpr std::size_t bufferSize = 7'000'000;
    auto buffer = std::make_unique<std::byte []>(bufferSize);
    std::pmr::monotonic_buffer_resource poolResource(buffer.get(), bufferSize);
    
    int beg{1}, end{1}, ans{};
    std::vector<int> prefix_xor(N + 1);
    node_t root(0);

    for (int i = 1; i <= N; ++i) {
        int n;
        task_in >> n;
        const int cur = prefix_xor[i - 1] ^ n;
        prefix_xor[i] = cur;

        // search
        int best{0};
        node_t* node = &root;
        for (int bit = high_bit; bit && node; bit >>= 1) {
            if (cur & bit) {
                if (node->left) {
                    node = node->left.get();
                    best = *node;
                } else
                    node = node->right.get();
            } else {
                if (node->right) {
                    node = node->right.get();
                    best = *node;
                } else
                    node = node->left.get();
            }
        }

        // check
        const int can = prefix_xor[best] ^ cur;
        if (ans < can) {
            ans = can;
            beg = best + 1;
            end = i;
        }

        // memo
        node = &root;
        for (int bit = high_bit; bit; bit >>= 1) {
            if (cur & bit) {
                if (!node->right)
                    node->right = node_ptr_t{new (poolResource.allocate(sizeof(node_t), alignof(node_t))) node_t(i)};
                node = node->right.get();
            } else {
                if (!node->left)
                    node->left = node_ptr_t{new (poolResource.allocate(sizeof(node_t), alignof(node_t))) node_t(i)};
                node = node->left.get();
            }
            *node = i;
        }
    }

    task_out << ans << ' ' << beg << ' ' << end << '\n';
}

int main(int, char**)
{
    // solution1();
    solution2();
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 cowxor.cpp -o cowxor.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address cowxor.cpp -o cowxor

Run:
cowxor.exe && type cowxor.out

Input:

5
1
0
5
4
2

Output:

6 4 5

*/
