
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

// Image Labeler
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008caea6/0000000000b76e11

static void solve() {
    int N, M;
    std::cin >> N >> M;
    std::vector<int> data(N);

    for (auto& n : data)
        std::cin >> n;

    const auto median = [](const std::vector<int>& data) -> float {
        const auto sz = data.size();
        if (sz & 1)
            return data[sz / 2];
        else
            return float(data[sz / 2 - 1] + data[sz / 2]) / 2;
    };

    std::sort(data.begin(), data.end());
    float singletons = std::accumulate(data.end() - M + 1, data.end(), 0LL);
    data.resize(N - M + 1);

    std::cout << singletons + median(data);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 image_labeller.cpp -o image_labeller.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address image_labeller.cpp -o image_labeller

Run:
image_labeller.exe < image_labeller.in

Input:

2
3 2
11 24 10
5 1
6 2 5 1 9

Output:

Case #1: 34.500000000
Case #2: 5.000000000

*/
