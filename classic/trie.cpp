
#include <array>
#include <assert.h>
#include <iostream>
#include <memory>
#include <string_view>
#include <unordered_map>

// Trie data structure
// https://en.wikipedia.org/wiki/Trie

template <typename V = int, int N = 26>
class trie_t {
  public:
    void put(const std::string_view& key, const V value) {
        assert(!key.empty());
        if (key.size() == 1)
            _store[key[0]] = value;
        else {
            const auto ind = char2index(key[0]);
            if (!_desc[ind])
                _desc[ind] = std::make_unique<trie_t>();
            _desc[ind].get()->put(key.substr(1), value);
        }
    }

    void pop(const std::string_view& key) {
        assert(!key.empty());
        if (key.size() == 1) {
            const auto found = _store.find(key[0]);
            if (found != _store.end())
                _store.erase(found);
            return;
        }

        const auto ind = char2index(key[0]);
        if (_desc[ind])
            _desc[ind].get()->pop(key.substr(1));
    }

    V get(const std::string_view& key) const {
        const auto ind = char2index(key[0]);
        if (key.size() == 1) {
            const auto found = _store.find(key[0]);
            return found == _store.end() ? NV : found->second;
        } else
            return _desc[ind] ? _desc[ind].get()->get(key.substr(1)): NV;
    }

    inline static const V NV = -1;

  private:
    inline int char2index(const char c) const { return c - 'A'; }
    std::array<std::unique_ptr<trie_t>, N> _desc;
    std::unordered_map<char, V> _store;
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
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 trie.cpp -o trie.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address trie.cpp -o trie

*/
