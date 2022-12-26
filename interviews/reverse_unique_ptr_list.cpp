
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

// Reverse List
// Classical coding question

static std::mt19937 g_gen{std::random_device{}()};

template <typename T = int>
static auto distribution(const T ubound) {
    std::uniform_int_distribution<T> dist(0, ubound - 1);
    return dist;
}

struct node_t {
    node_t(int p, node_t* n) : payload(p), next(n) {}
    int payload;
    std::unique_ptr<node_t> next;
};

/**
 * @brief Reverses single linked list
 * @remark Original list is destroyed
 */
auto reverse(std::unique_ptr<node_t> list) {
    decltype(list) reversed{};
    while (list) {
        auto cur = list.release();
        list = std::move(cur->next);
        cur->next = std::move(reversed);
        reversed.reset(cur);
    }

    return reversed;
}

std::unique_ptr<node_t> vec2list(std::vector<int> vec) {
    std::unique_ptr<node_t> ret{};
    std::reverse(vec.begin(), vec.end()); // that's why passing by value
    for (const auto v : vec) {
        auto node = std::make_unique<node_t>(v, ret.release());
        ret = std::move(node);
    }

    return ret;
}

std::vector<int> list2vec(std::unique_ptr<node_t> list) {
    std::vector<int> ret;
    while (list) {
        ret.push_back(list->payload);
        list = std::move(list->next);
    }

    return ret;
}

constinit const int N = 10000;

int main(int, char**) {
    // arrange
    std::vector<int> data(N);
    auto dist = distribution<int64_t>(1e09);
    for (auto& d : data)
        d = dist(g_gen);

    // act
    auto list = vec2list(data);
    auto reversed_list = reverse(std::move(list));
    auto reversed_vector = list2vec(std::move(reversed_list));
    std::reverse(reversed_vector.begin(), reversed_vector.end());

    // assert
    assert(reversed_vector == data);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 reverse_unique_ptr_list.cpp -o reverse_unique_ptr_list.exe

*/
