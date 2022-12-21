
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
    node_t(int p, std::shared_ptr<node_t> n) : payload(p), next(n) {}
    int payload;
    std::shared_ptr<node_t> next;
};

/**
 * @brief Reverses single linked list
 * @remark Original list is destroyed
 */
std::shared_ptr<node_t> reverse(std::shared_ptr<node_t> list) {
    std::shared_ptr<node_t> reversed{};
    while (list) {
        auto cur = list;
        list = list->next;
        cur->next = reversed;
        reversed = cur;
    }

    return reversed;
}

std::shared_ptr<node_t> vec2list(std::vector<int> vec) {
    std::shared_ptr<node_t> ret{};
    std::reverse(vec.begin(), vec.end()); // that's why passing by value
    for (const auto v : vec) {
        auto node = std::make_shared<node_t>(v, ret);
        ret = std::move(node);
    }

    return ret;
}

std::vector<int> list2vec(std::shared_ptr<node_t> list) {
    std::vector<int> ret;
    while (list) {
        ret.push_back(list->payload);
        list = list->next;
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
    const auto list = vec2list(data);
    const auto reversed_list = reverse(list);
    auto reversed_vector = list2vec(reversed_list);
    std::reverse(reversed_vector.begin(), reversed_vector.end());

    // assert
    assert(reversed_vector == data);
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 reverse_list.cpp -o reverse_list.exe

*/
