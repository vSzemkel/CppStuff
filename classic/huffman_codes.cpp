
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Huffman codes

template <typename T>
using min_pq_t = std::priority_queue<T, std::vector<T>, std::greater<>>;

int main(int, char**)
{
    // read char statistics for this source file
    char c;
    int uncompresed_length{};
    std::unordered_map<char, int> freq;
    std::ifstream text("huffman_codes.cpp", std::ios_base::binary);
    while (text.good()) {
        text >> std::noskipws >> c;
        ++uncompresed_length;
        ++freq[c];
    }

    // enumerate chars and build mpq
    short next{0};
    std::unordered_map<short, char> rev_index;
    min_pq_t<std::pair<int, short>> mpq; // { freq, decltype(next)}
    for (const auto& f : freq) {
        rev_index[next] = f.first;
        mpq.emplace(f.second, next++);
    }

    // build the prefix tree
    const int char_count = next;
    std::vector<std::array<short, 2>> tree(2 * next - 1);
    for (int z = next - 1; z; --z) { 
        const auto l = mpq.top(); mpq.pop();
        const auto r = mpq.top(); mpq.pop();
        tree[next][0] = l.second;
        tree[next][1] = r.second;
        mpq.emplace(l.first + r.first, next++);
    }
    assert(mpq.size() == 1);

    // DFS the tree and save codes
    std::vector<std::pair<int, std::string>> stack(1, {next - 1, {}});
    std::unordered_map<char, std::string> char2code;
    std::unordered_map<std::string, char> code2char;
    while (!stack.empty()) {
        const auto n = stack.back(); stack.pop_back();
        if (n.first < char_count) {
            const char c = rev_index[n.first];
            char2code[c] = n.second;
            code2char[n.second] = c;
        } else {
            stack.emplace_back(tree[n.first][0], n.second + '0');
            stack.emplace_back(tree[n.first][1], n.second + '1');
        }
    }

    // encode
    std::stringstream compressed;
    text.clear();
    text.seekg(0);
    while (text.good()) {
        text >> std::noskipws >> c;
        compressed << char2code[c];
    }
    text.close();

    int compressed_size = compressed.tellp();
    std::cout << compressed.str() << "\n\nNumber of distinct characters: " << char_count << "\nOriginal length: " << uncompresed_length << "\nCompressed length: " << (compressed_size + 7) / 8 << '\n';

    // decode
    compressed.seekg(0);
    std::string current_prefix{};
    for (; compressed_size; --compressed_size) {
        compressed >> c;
        current_prefix += c;
        if (const auto found = code2char.find(current_prefix); found != code2char.end()) {
            std::cout << found->second;
            current_prefix.clear();
        }
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 huffman_codes.cpp -o huffman_codes.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++20 -fsanitize=address huffman_codes.cpp -o huffman_codes

Output:

Number of distinct characters: 69
Original length: 3211
Compressed length: 1965

#include <assert.h>
#include <iostream>
...

*/
