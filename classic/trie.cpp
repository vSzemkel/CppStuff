
#include <assert.h>
#include <iostream>
#include <string_view>
#include <unordered_map>

// Trie data structure
// https://en.wikipedia.org/wiki/Trie
// see: /leetcode/word_search_ii.cpp

template <typename V = int>
class trie_t {
  public:
    V get(const std::string_view& key) const {
        if (key.empty())
            return _store;

        auto found = _desc.find(key[0]);
        if (found == _desc.end())
            return NV;

        return found->second.get(key.substr(1));
    }

    void put(const std::string_view& key, const V value) {
        if (key.empty())
            _store = value;
        else
            _desc[key[0]].put(key.substr(1), value);
    }

    void pop(const std::string_view& key) {
        if (key.empty()) {
            _store = NV;
            return;
        }

        const auto found = _desc.find(key[0]);
        if (found == _desc.end())
            return;

        found->second.pop(key.substr(1));
    }

    inline static const V NV = -1;

  private:
    std::unordered_map<char, trie_t> _desc;
    V _store{NV};
};

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    trie_t trie;
    assert(trie.get("ABRA") == -1);
    trie.put("ABRA", 123);
    assert(trie.get("ABRA") == 123);
    trie.put("ABRAKADABRA", 456);
    assert(trie.get("ABRAKADABRA") == 456);
    assert(trie.get("ABRA") == 123);
    trie.pop("ABRA");
    assert(trie.get("ABRA") == -1);
    assert(trie.get("ABRAKADABRA") == 456);
    trie.put("ABRAKADABRA", 789);
    assert(trie.get("ABRAKADABRA") == 789);
    assert(trie.get("KADA") == trie_t<>::NV);
    trie.pop("ABNOTEXISTSAB");
    
    trie.put("X", 1);
    trie.put("XYZ", 3);
    assert(trie.get("X") == 1);
    assert(trie.get("XY") == trie_t<>::NV);
    assert(trie.get("XYZ") == 3);
    trie.put("XY", 2);
    assert(trie.get("XY") == 2);
    trie.pop("XY");
    assert(trie.get("X") == 1);
    assert(trie.get("XY") == trie_t<>::NV);
    assert(trie.get("XYZ") == 3);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 trie.cpp -o trie.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address trie.cpp -o trie

*/
