
#include <assert.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// Huffman codes

template <typename T>
using min_pq_t = std::priority_queue<T, std::vector<T>, std::greater<>>;

int main(int, char**)
{
    // read char statistics for this source file
    std::unordered_map<char, int> freq;
    std::ifstream text("huffman_codes.cpp");
    for (std::istream_iterator<char> it{text}; it != std::istream_iterator<char>{}; ++it)
        ++freq[*it];
    text.close();

    // enumerate chars and build mpq
    int next{0};
    std::unordered_map<char, int> node;
    min_pq_t<std::pair<int, char>> mpq;
    for (const auto& f : freq) {
        node[f.first] = next;
        mpq.emplace(f.second, next++);
    }

    // build the prefix tree
    const int char_count = next;
    std::vector<std::vector<int>> tree(2 * next - 1);
    for (int z = next - 1; z; --z) { 
        const auto l = mpq.top(); mpq.pop();
        const auto r = mpq.top(); mpq.pop();
        tree[next].push_back(l.second);
        tree[next].push_back(r.second);
        mpq.emplace(l.first + r.first, next++);
    }
    assert(mpq.size() == 1);

    // DFS the tree and save codes
    std::string prefix{};
    std::vector<std::pair<int, std::string>> stack(1, {next - 2, {}});
    std::unordered_map<char, std::string> code;
    while (!stack.empty()) {
        const auto n = stack.back(); stack.pop_back();
        if (n.first < char_count)
            code[n.first] = n.second;
        else {
            stack.emplace_back(tree[n.first][0], n.second + '0');
            stack.emplace_back(tree[n.first][1], n.second + '1');
        }
    }

    std::ifstream input("huffman_codes.cpp");
    for (std::istream_iterator<char> it{input}; it != std::istream_iterator<char>{}; ++it)
        std::cout << code[*it];
    input.close();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 huffman_codes.cpp -o huffman_codes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address huffman_codes.cpp -o huffman_codes

Output:

010001001000100100010010001001000100100010..

*/
