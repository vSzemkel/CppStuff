#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

    int N, std::cin >> N;
    g_input.resize(N);
    for (auto& n : g_input)
        std::cin >> n;
    // normalize unique input
    std::vector<int> g_input_ord(N);
    std::iota(g_input_ord.begin(), g_input_ord.end(), 0);
    std::sort(g_input_ord.begin(), g_input_ord.end(), [](const int i, const int j){
        return g_input[i] < g_input[j];
    });
    for (int i = 0; i < N; ++i)
        g_input[g_input_ord[i]] = i + 1;


// {41, 3, -81, 6, 11} -> {5, 2, 1, 3, 4}