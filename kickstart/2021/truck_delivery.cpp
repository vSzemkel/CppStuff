
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// Truck delivery
// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000435a5b/000000000077a885


class city_t {
  public:
    int city;
    int road_to;
    int limit;
    int64_t toll;
};

static std::vector<city_t> graph2tree(const std::vector<std::vector<city_t>>& roads, const int root) {
    const int size = (int)roads.size();
    assert(0 <= root && root < size);
    std::vector<city_t> stack, parents(size);
    std::vector<bool> visited(size);
    parents[root] = stack.emplace_back(city_t{root, -1, 0, 0});
    visited[root] = true;
    while (!stack.empty()) {
        const auto curr = stack.back();
        stack.pop_back();
        for (auto child : roads[curr.city]) {
            if (child.road_to != curr.city)
                std::swap(child.city, child.road_to);
            if (visited[child.city]) continue;
            visited[child.city] = true;
            parents[child.city] = child;
            stack.push_back(child);
        }
    }

    return parents;
}

static void solve() {
    int N, Q; std::cin >> N >> Q;
    std::vector<std::vector<city_t>> roads(N);
    for (int i = 0; i < N - 1; ++i) {
        int c1, c2; std::cin >> c1 >> c2;
        int L; int64_t A; std::cin >> L >> A;
        roads[--c1].push_back({c1, --c2, L, A});
        roads[c2].push_back({c2, c1, L, A});
    }

    const std::vector<city_t> parents = graph2tree(roads, 0);

    for (int i = 0; i < Q; ++i) {
        int c; int64_t weight, gcd{0};
        std::cin >> c >> weight; --c;
        auto road = parents[c];
        while (road.city > 0) {
            if (weight >= road.limit)
                gcd = std::gcd(gcd, road.toll);

            road = parents[road.road_to];
        }

        std::cout << gcd << " ";
    }
}

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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 truck_delivery.cpp -o truck_delivery.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address truck_delivery.cpp -o truck_delivery.o

Run:
truck_delivery.exe < truck_delivery.in

Input:

2
7 5
2 1 2 4
2 3 7 8
3 4 6 2
5 3 9 9
2 6 1 5
7 1 5 7
5 10
5 8
4 1
6 1
7 6
3 2
1 2 2 10
3 2 3 5
3 2
3 3

Output:

Case #1: 1 4 0 5 7 
Case #2: 10 5

*/
