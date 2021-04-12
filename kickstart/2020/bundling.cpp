
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Bundling
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc7/00000000001d3ff3

struct trie_t {
    void insert(const std::string& s) {
        auto n = this;
        for (const char c : s) {
            int ind{0}, size = (int)n->_chars.size();
            for(; ind < size; ++ind)
                if (n->_chars[ind] == c)
                    break;

            if (ind == size) {
                n->_chars.push_back(c);
                n->_nodes.push_back(trie_t{});
            }

            n = &n->_nodes[ind];
            n->_count++;
        }
    }

    void insert_recursion(const std::string_view s) { // WITH std::string CAN NOT PASS SET2 (MLE)
        if (!s.empty()) {
            const char c = s[0];
            auto it = std::find_if(_chars.begin(), _chars.end(), [c](const auto& n){
                return n == c;
            });

            const bool found = it != _chars.end();
            if (!found) {
                _chars.push_back(c);
                _nodes.push_back(trie_t{});
            }

            auto& n = found ? _nodes[it - _chars.begin()] : _nodes.back();
            n._count++;
            n.insert_recursion(s.substr(1));
        }
    };

    int64_t group(const int size) const {
        int64_t ret = _count / size; // intfloor
        for (const auto& n : _nodes)
            ret += n.group(size);
        return ret;
    }

    std::vector<trie_t> _nodes;
    std::vector<char> _chars;
    int _count{0};
};

static void solve() {
    int N, K; std::cin >> N >> K;
    trie_t tr;
    for (int i = 0; i < N; ++i) {
        std::string s; std::cin >> s;
        tr.insert(s);
    }

    std::cout << tr.group(K);
}

/*static void greedy_doesnt_work () {
    int N, K; std::cin >> N >> K;
    std::vector<std::string> data(N);
    std::vector<bool> taken(N);
    for (int i = 0; i < N; ++i)
        std::cin >> data[i];

    std::vector<string_hasher<>> hashes;
    for (int i = 0; i < N; ++i)
        hashes.push_back(string_hasher{data[i]});

    size_t max_len{0};
    for (int i = 0; i < N; ++i)
        max_len = std::max(max_len, data[i].size());

    int64_t ret{0};
    for (int i = max_len; i > 0; --i) {
        std::vector<std::pair<int, int>> i_len_hashes(N);
        for (int j = 0; j < N; ++j)
            if (taken[j] || (int)data[j].size() < i)
                i_len_hashes[j] = {-1, j};
            else
                i_len_hashes[j] = {hashes[j].hash(0, i), j};
        std::sort(i_len_hashes.begin(), i_len_hashes.end());
        for (int k = 0; k < N - K + 1; ++k) {
            const int match = i_len_hashes[k].first;
            if (match == -1) continue;
            int end{k};
            while (end < N && i_len_hashes[end].first == match) ++end;
            if (end - k >= K) {
                ret += i;
                for (int l = k; l < k + K; ++l)
                    taken[i_len_hashes[l].second] = true;
                k += K;
            }
        }
    }

    std::cout << ret;
}*/

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 bundling.cpp -o bundling.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address bundling.cpp -o bundling.o

Run:
py.exe interactive_runner.py py.exe bundling_testing_tool.py 1 -- bundling.exe
bundling.exe < bundling.in

Input:

2
2 2
KICK
START
8 2
G
G
GO
GO
GOO
GOO
GOOO
GOOO
6 3
RAINBOW
FIREBALL
RANK
RANDOM
FIREWALL
FIREFIGHTER

Output:

Case #1: 0
Case #2: 10
Case #3: 6

*/
