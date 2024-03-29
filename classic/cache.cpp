
#include <cassert>
#include <iostream>
#include <optional>
#include <map>
#include <utility>

template <typename K, typename V>
struct cache_t {
    void add(const K& key, const V& value) {
        _cache[key] = value;
    }

    void clear() { _cache.clear(); }

    std::optional<V> find(const K& key) {
        auto cached = _cache.find(key);
        if (cached == _cache.end())
            return std::nullopt;
        return cached->second;
    }
  private:
    std::map<K, V> _cache;
};

int main(int, char**)
{
    cache_t<std::pair<int, int>, int> cache;

    assert(cache.find({2,5}) == std::nullopt);
    cache.add({2, 5}, 177);
    assert(cache.find({2,5}) == 177);

    const auto cached = cache.find({2, 5});
    if (cached != std::nullopt)
        std::cout << cached.value() << ' ';

    cache_t<std::pair<int, int>, int> cache2[2];
    for (int i = 0; i < 99; ++i) {
        auto& cur_cache = cache2[i & 1];
        auto& prev_cache = cache2[(i & 1) ^ 1];
        cur_cache.clear();
        // fill cur_cache based on prev_cache
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 g_cache.cpp -o g_cache.exe
*/