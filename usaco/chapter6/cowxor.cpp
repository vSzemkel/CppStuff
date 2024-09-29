/*
ID: marcin.8
LANG: C++
TASK: cowxor
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=VDkbdceba9s&S=cowxor
*/

#include <fstream>
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

int main(int, char**)
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
