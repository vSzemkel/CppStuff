
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Word Search II
// https://leetcode.com/problems/word-search-ii/description

int R, C, W;
const int dr[4] = {1, 0, -1, 0};
const int dc[4] = {0, 1, 0, -1};

class trie_t {
  public:
     const trie_t& node(const char letter) const {
        const auto found = _desc.find(letter);
        if (found == _desc.end())
            return *invalid;

        return found->second;
    }

    bool valid() const {
        return _exists;
    }

    bool terminal() const {
        return _terminal;
    }

    void put(std::string_view key) {
        _exists = true;
        if (key.empty())
            _terminal = true;
        else {
            auto& child = _desc[key.front()];
            child._parent = this;
            child.put(key.substr(1));
        }
    }

    void remove_up(std::string_view key) {
        _terminal = false;
        if (!_desc.empty())
            return;
        if (_parent) {
            _parent->_desc.erase(key.back());
            _parent->remove_up(key.substr(0, key.size() - 1));
        }
    }

    static std::unique_ptr<trie_t> invalid;

  private:
    trie_t* _parent{};
    std::unordered_map<char, trie_t> _desc;
    bool _exists{}, _terminal{};
};

auto trie_t::invalid = std::make_unique<trie_t>();

std::vector<std::string> findWords(std::vector<std::vector<char>>& board, std::vector<std::string>& words) {
    trie_t trie;
    for (const auto& w : words)
        trie.put(w);

    std::unordered_map<std::string, trie_t*> nodes;

    using check_t = std::function<void(int, int, int, std::string, const trie_t&)>;
    const check_t check = [&](int r, int c, int d, std::string s, const trie_t& t) {
        const auto org = std::exchange(board[r][c], '*'); 
        for (int z = 3, nd = (d + 3) % 4; z; --z, nd = (nd + 1) % 4) {
            const int nr = r + dr[nd];
            const int nc = c + dc[nd];
            if (0 <= nr && nr < R && 0 <= nc && nc < C) {
                const auto cur = board[nr][nc];
                const auto ntrie = t.node(cur);
                if (ntrie.valid())
                    check(nr, nc, d, s + cur, ntrie);
            }
        }
        board[r][c] = org;

        if (t.terminal())
            nodes.emplace(std::move(s), const_cast<trie_t*>(&t));
    };

    std::unordered_set<std::string> ans;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const char cur = board[r][c];
            for (int dir = 0; dir < 4; ++dir) {
                const auto ntrie = trie.node(cur);
                if (ntrie.valid()) {
                    check(r, c, dir, std::string(1, cur), ntrie);
                    for (auto&& [word, node] : nodes) {
                        // node->remove_up(word); TRICKY
                        ans.insert(std::move(word));
                    }
                    nodes.clear();
                }
            }
        }

    return {ans.begin(), ans.end()};
}

static void io_handler() {
    std::cin >> R >> C;
    std::vector<std::vector<char>> board(R, std::vector<char>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            std::cin >> board[r][c];
    std::cin >> W;
    std::vector<std::string> words(W);
    for (auto& w : words)
        std::cin >> w;

    const auto answer = findWords(board, words);
    for (auto& w : answer)
        std::cout << '\n' << w;
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; io_handler(); std::cout << '\n';
    }
}
/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 word_search_ii.cpp -o word_search_ii.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address word_search_ii.cpp -o word_search_ii

Run:
word_search_ii.exe < word_search_ii.in

Input:

1
4 4
oaan
etae
ihkr
iflv
4
oath
pea
eat
rain

Output:

oath
eat

*/
