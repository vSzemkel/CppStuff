
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
const int dr[8] = {1, 0, -1, 0, 1, 0, -1, 0};
const int dc[8] = {0, 1, 0, -1, 0, 1, 0, -1};

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

    std::string label() const {
        return _label;
    }

    bool terminal() const {
        return _terminal;
    }

    void put(std::string_view key) {
        trie_t *cur = this;
        for (const char c : key) {
            auto& child = cur->_desc[c];
            child._exists = true;
            child._parent = cur;
            cur = &child;
        }

        cur->_terminal = true;
        cur->_label = key;
    }

    void remove_up() {
        _terminal = false;
        if (!_desc.empty())
            return;
        auto c = _label.end();
        for (auto p = _parent; p; p = p->_parent) {
            p->_desc.erase(*--c);
            if (p->_terminal || !p->_desc.empty())
                return;
        }
    }

    static std::unique_ptr<trie_t> invalid;

  private:
    trie_t* _parent{};
    std::string _label{};
    std::unordered_map<char, trie_t> _desc;
    bool _exists{}, _terminal{};
};

auto trie_t::invalid = std::make_unique<trie_t>();

std::vector<std::string> findWords(std::vector<std::vector<char>>& board, std::vector<std::string>& words) {
    trie_t trie;
    for (const auto& w : words)
        trie.put(w);

    std::unordered_set<trie_t*> nodes;
    using check_t = std::function<void(int, int, int, const trie_t&)>;
    const check_t check = [&](int r, int c, int d, const trie_t& t) {
        const auto org = std::exchange(board[r][c], '*'); 
        for (int z = 3, nd = (d + 3) % 4; z; --z, ++nd) {
            const int nr = r + dr[nd];
            const int nc = c + dc[nd];
            if (0 <= nr && nr < R && 0 <= nc && nc < C) {
                const auto cur = board[nr][nc];
                const auto& ntrie = t.node(cur);
                if (ntrie.valid())
                    check(nr, nc, nd, ntrie);
            }
        }
        board[r][c] = org;

        if (t.terminal())
            nodes.insert(const_cast<trie_t*>(&t));
    };

    std::vector<std::string> ans;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            const char cur = board[r][c];
            for (int dir = 0; dir < 4; ++dir) {
                const auto& ntrie = trie.node(cur);
                if (ntrie.valid()) {
                    check(r, c, dir, ntrie);
                    for (auto& node : nodes) {
                        ans.push_back(node->label());
                        node->remove_up(); // TRICKY PART
                    }
                    nodes.clear();
                }
            }
        }

    return ans;
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

3
2 4
oaan
etae
2
oa
oaa
3 3
abc
aed
afg
6
abcdefg
gfedcbaaa
eaabcdgfa
befa
dgc
ade
12 12
bcdefghijklm
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
3
aaaaaaaaaaac
aaaaaaaaaaak
aaaaaaaaaaat

Output:

Case #1: 
oaa
oa
Case #2:
abcdefg
befa
eaabcdgfa
gfedcbaaa
Case #3:
aaaaaaaaaaak
aaaaaaaaaaac

*/
