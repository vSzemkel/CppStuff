
#include <cassert>
#include <iostream>
#include <map>
#include <string_view>
#include <vector>

// Suffix Automaton
// https://cp-algorithms.com/string/suffix-automaton.html#implementation

struct suff_auto_state_t {
    std::map<char, int> next; // transition letter -> state
    int len;                  // length of the prefix leading to this state
    int link;                 // link to the longest state in different endpos-equivalence class
    bool terminal;            // suffix accepting state
};

struct suff_auto_t {
    suff_auto_t(const std::string_view s) {
        assert(!s.empty());
        _nodes.reserve(s.size() * 2);
        _nodes.push_back(suff_auto_state_t{{}, 0, -1, {}});
        for (const char c : s)
            extend(c);

        for (auto suffix = _last; ~suffix; suffix = _nodes[suffix].link)
            _nodes[suffix].terminal = true;
    }

    int find(const std::string_view t) {
        int cur{0};
        bool found{true};
        const char* c = t.data();
        const int len = int(t.size());
        for (int z = len; z; --z) {
            const auto next = _nodes[cur].next;
            const auto it = next.find(*c);
            if (it == next.end())
                return -1;

            cur = it->second;
            ++c;
        }

        return _nodes[cur].len - len;
    }

    std::vector<suff_auto_state_t> _nodes;
    int _last{0};
    int _size{1};

  private:
    void extend(const char c) {
        const auto cur = _size++;
        _nodes.push_back(suff_auto_state_t{{}, _nodes[_last].len + 1, 0, {}});

        auto p = _last;
        while (~p && _nodes[p].next.count(c) == 0) {
            _nodes[p].next[c] = cur;
            p = _nodes[p].link;
        }

        if (~p) { // if (p == -1) link already set to 0
            const auto q = _nodes[p].next[c];
            if (_nodes[p].len + 1 == _nodes[q].len) {
                _nodes[cur].link = q;
            } else {
                const auto clone = _size++;
                _nodes.push_back(suff_auto_state_t{_nodes[q].next, _nodes[p].len + 1, _nodes[q].link, {}});
                while (~p && _nodes[p].next[c] == q) {
                    _nodes[p].next[c] = clone;
                    p = _nodes[p].link;
                }
                _nodes[q].link = _nodes[cur].link = clone;
            }
        }

        _last = cur;
    }
};

int main(int, char**)
{
    suff_auto_t sa{"abcbc"};
    assert(sa._nodes[sa._last].len == 5);
    assert(sa._size == 8);

    const int offset{27};
    std::string_view text{"ahdynklzanaskwydryosbqfntlhsiusiakagnwmcbarqwacjyplxmyaw"};
    std::string_view patt(text.data() + offset, 7);
    sa = text;

    // Check if substring is not in text
    assert(sa.find("dupa") == -1);
    // First occurence position
    assert(sa.find(patt) == offset);
    // Find empty string
    assert(sa.find("") == 0);

    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 suffix_automaton.cpp -o suffix_automaton.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address suffix_automaton.cpp -o suffix_automaton

*/
